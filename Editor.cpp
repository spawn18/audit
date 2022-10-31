#include "Editor.hpp"
#include "./ui_editor.h"

#include <QDebug>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QShortcut>
#include <QMessageBox>

#include <algorithm>

#include "Track.hpp"
#include "Utils.hpp"


Editor::Editor(QWidget *parent) : QMainWindow(parent), ui(new Ui::Editor)
{
    ui->setupUi(this);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionProps, SIGNAL(triggered()), this, SLOT(properties()));
    connect(ui->toggle, SIGNAL(clicked(bool)), this, SLOT(toggle(bool)));
    connect(ui->stop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->actionCut, SIGNAL(triggered()), this, SLOT(cut()));
    connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->actionErase, SIGNAL(triggered()), this, SLOT(erase()));
    connect(ui->actionWhite_noise, SIGNAL(triggered()), this, SLOT(whiteNoise()));
    connect(ui->menuGenerate_tone, SIGNAL(triggered(QAction*)), this, SLOT(generateTone(QAction*)));
    connect(ui->actionReverse, SIGNAL(triggered()), this, SLOT(reverse()));
    connect(ui->actionFade_in, SIGNAL(triggered()), this, SLOT(fadeIn()));
    connect(ui->actionFade_out, SIGNAL(triggered()), this, SLOT(fadeOut()));

    connect(ui->volume, SIGNAL(valueChanged(int)), &m_audio, SLOT(setVolume(int)));
    connect(&m_audio, SIGNAL(played()), this, SLOT(handlePlayed()));
    connect(&m_audio, SIGNAL(ended()), this, SLOT(handleEnded()));
    connect(ui->track, SIGNAL(clicked(double)), this, SLOT(handleTrackClick(double)));

    enableUI(false);
}

Editor::~Editor()
{
    delete ui;
}

void Editor::open()
{
    try
    {
        QString path = QFileDialog::getOpenFileName(this, "Open file", QStandardPaths::displayName(QStandardPaths::DesktopLocation), "Audio files (*.wav)");
        if(!path.isNull())
        {
            if(m_file.isOpen())
                m_file.close();

            m_file.load(path);
            m_file.open(QIODevice::ReadOnly | QIODevice::Unbuffered);

            m_audio.setSource(&m_file);

            ui->track->setEnabled(true);
            ui->track->updateSelection(Selection(0.0, 1.0), m_file);
            ui->track->setPosition(0);
            ui->track->setSelection(Selection(0, 0));

            ui->time->setText(Time(0).asString());
            ui->toggle->setChecked(false);

            m_isBufferSet = false;
            setWindowTitle(APPLICATION_NAME + " - " + path);

            enableUI(true);
        }
    }
    catch(std::exception& ex)
    {
        QMessageBox::information(this, "Error", ex.what());
    }
}


void Editor::properties()
{
    Properties m_properties;
    m_properties.setProperties(m_file.sampleRate(), m_file.sampleSize(), m_file.duration(), m_file.channels(), m_file.byteRate());
    m_properties.exec();
}

void Editor::toggle(bool checked)
{
    if(checked)
        m_audio.play();
    else
        m_audio.pause();
}

void Editor::stop()
{
    m_audio.stop();
    ui->track->setPosition(0);
    ui->toggle->setChecked(false);
    ui->time->setText(Time(0).asString());
}

void Editor::handlePlayed()
{
    Time time = m_audio.getTime();
    ui->time->setText(time.asString());

    double position = static_cast<double>(time.asMiliseconds()) / m_file.duration().asMiliseconds();
    ui->track->setPosition(position);
}

void Editor::handleEnded()
{
    ui->toggle->setChecked(false);
    ui->track->setPosition(0);
    ui->time->setText(Time(0).asString());
}

void Editor::handleTrackClick(double position)
{
    Time time = milliseconds(position * m_file.duration().asMiliseconds());
    ui->time->setText(time.asString());
    m_audio.setTime(time);
}

void Editor::save()
{
    m_file.save();
}

void Editor::enableUI(bool state)
{
    ui->menuEffects->setEnabled(state);
    ui->menuEdit->setEnabled(state);
    ui->actionProps->setEnabled(state);
    ui->actionSave->setEnabled(state);
    ui->toggle->setEnabled(state);
    ui->stop->setEnabled(state);
    ui->volume->setEnabled(state);
}

void Editor::reverse()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        int diff = end - begin;
        for(int i = 0; i <= diff/2.0; i++)
            std::swap(m_file[begin + i], m_file[end - i]);

        ui->track->updateSelection(selection, m_file);
    }
}

void Editor::erase()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        for(int i = selection.begin; i <= selection.end; i++)
            m_file[i] = 0;

        ui->track->updateSelection(selection, m_file);
    }
}

void Editor::paste()
{
    if(m_isBufferSet)
    {
        double pos = ui->track->getPosition();
        qint64 sample = pos * (m_file.totalSamples()-1);
        double end = static_cast<double>(sample + m_buffer.size()) / (m_file.totalSamples()-1);

        if(sample + m_buffer.size() < m_file.totalSamples())
        {
            for(int i = 0; i < m_buffer.size(); i++)
                m_file[sample+i] = m_buffer[i];

            Selection selection(pos, end);
            ui->track->updateSelection(selection, m_file);
        }
    }
}

void Editor::copy()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        m_buffer.resize(end - begin);

        for(int i = begin; i <= end; i++)
            m_buffer[i-begin] = m_file[i];

        m_isBufferSet = true;
    }
}

void Editor::cut()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        m_buffer.resize(end - begin);

        for(int i = begin; i <= end; i++)
        {
            m_buffer[i-begin] = m_file[i];
            m_file[i] = 0;
        }

        m_isBufferSet = true;    
        ui->track->updateSelection(selection, m_file);
    }
}

void Editor::generateTone(QAction *action)
{
    if(ui->track->isSelected())
    {
        QString name = action->objectName();

        double frequency;

        if      (name == "actionC") frequency = FREQUENCY_C;
        else if (name == "actionD") frequency = FREQUENCY_D;
        else if (name == "actionE") frequency = FREQUENCY_E;
        else if (name == "actionF") frequency = FREQUENCY_F;
        else if (name == "actionG") frequency = FREQUENCY_G;
        else if (name == "actionA") frequency = FREQUENCY_A;
        else if (name == "actionH") frequency = FREQUENCY_H;

        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        for(int i = begin; i <= end; i++)
        {
            double sec = i / static_cast<double>(m_file.sampleRate());
            m_file[i] = EFFECT_VOLUME_MUL * MAX_SAMPLE_VALUE * sin(frequency*2*PI*sec);
        }

        ui->track->updateSelection(selection, m_file);
    }
}

void Editor::fadeIn()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        for(int i = begin; i <= end; i++)
        {
            double factor = 1.0 - static_cast<double>(end - i) / (end - begin);
            m_file[i] *= factor;
        }

        ui->track->updateSelection(selection, m_file);
    }
}

void Editor::fadeOut()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        for(int i = begin; i <= end; i++)
        {
            double factor = static_cast<double>(end - i) / (end - begin);
            m_file[i] *= factor;
        }

        ui->track->updateSelection(selection, m_file);
    }
}

void Editor::whiteNoise()
{
    if(ui->track->isSelected())
    {
        Selection selection = ui->track->getSelection();

        qint64 begin = selection.begin * (m_file.totalSamples()-1);
        qint64 end = selection.end * (m_file.totalSamples()-1);

        srand(time(0));
        for(int i = begin; i <= end; i++)
            m_file[i] = EFFECT_VOLUME_MUL * random(MIN_SAMPLE_VALUE, MAX_SAMPLE_VALUE);

        ui->track->updateSelection(selection, m_file);
    }
}
