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
    ui->labelDuration->setText(QString("%1:%2:%3").arg(duration.getHours(), 2, 10, QLatin1Char('0'))
                                                     .arg(duration.getMinutes(), 2, 10, QLatin1Char('0'))
                                                     .arg(duration.getSeconds(), 2, 10, QLatin1Char('0')));
}

Properties::~Properties()
{
    delete ui;
}
