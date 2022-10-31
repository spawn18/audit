#ifndef UTILS_HPP
#define UTILS_HPP

#include <QColor>
#include <QString>

extern const long double PI;

extern const double FREQUENCY_C;
extern const double FREQUENCY_D;
extern const double FREQUENCY_E;
extern const double FREQUENCY_F;
extern const double FREQUENCY_G;
extern const double FREQUENCY_A;
extern const double FREQUENCY_H;

extern const short WAVE_FORMAT_PCM;

extern const QColor COLOR_WAVEFORM_MEAN;
extern const QColor COLOR_WAVEFORM_MAX;
extern const QColor COLOR_CURSOR;
extern const QColor COLOR_POSITION;

extern const int SELECTION_THRESHOLD;

extern const qint16 MAX_SAMPLE_VALUE;
extern const qint16 MIN_SAMPLE_VALUE;

extern const double EFFECT_VOLUME_MUL;

extern const QString APPLICATION_NAME;

int random(int a, int b);

#endif
