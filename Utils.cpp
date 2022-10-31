#include "Utils.hpp"

#include <cmath>

const long double PI = 3.14159265358979323846;

const double FREQUENCY_C = 261.625565;
const double FREQUENCY_D = 293.665;
const double FREQUENCY_E = 329.628;
const double FREQUENCY_F = 349.228;
const double FREQUENCY_G = 391.995;
const double FREQUENCY_A = 440;
const double FREQUENCY_H = 493.88;

const short WAVE_FORMAT_PCM = 0x0001;

const QColor COLOR_WAVEFORM_MEAN = QColor(135,158,255);
const QColor COLOR_WAVEFORM_MAX = QColor(100,100,225);
const QColor COLOR_CURSOR = QColor(255,255,255);
const QColor COLOR_POSITION = QColor(255,255,255);

const int SELECTION_THRESHOLD = 1;

const qint16 MAX_SAMPLE_VALUE = 32767;
const qint16 MIN_SAMPLE_VALUE = -32768;

const QString APPLICATION_NAME = "AuDit";

const double EFFECT_VOLUME_MUL = 0.5;

int random(int a, int b)
{
    return a + (rand() / (static_cast<double>(RAND_MAX)) * (b-a));
}
