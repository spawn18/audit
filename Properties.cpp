#include "Properties.hpp"
#include "ui_Properties.h"

Properties::Properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void Properties::setProperties(int sampleRate, int sampleSize, Time duration, int channels, int byteRate)
{
    ui->labelByteRate->setText(QString::number(byteRate));
    ui->labelSampleRate->setText(QString::number(sampleRate));
    ui->labelChannels->setText(QString::number(channels));
    ui->labelSampleSize->setText(QString::number(sampleSize));
    ui->labelDuration->setText(duration.asString());
}

Properties::~Properties()
{
    delete ui;
}
