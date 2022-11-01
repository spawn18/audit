#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <QWidget>
#include <QDialog>

#include "Time.hpp"

namespace Ui {
class Properties;
}

class Properties : public QDialog
{
    Q_OBJECT

public:
    explicit Properties(QWidget *parent = nullptr);
            ~Properties();

    void setProperties(int sampleRate, int sampleSize, Time duration, int channels, int byteRate);

private:
    Ui::Properties *ui;
};

#endif
