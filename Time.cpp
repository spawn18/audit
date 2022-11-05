#include "Time.hpp"

#include <QString>

Time::Time(qint64 mcs)
{
    m_microseconds = mcs;
}

qint64 Time::asMicroseconds() const
{
    return m_microseconds;
}

QString Time::asString() const
{
    int hours = (m_microseconds / 3'600'000'000) % 60;
    int minutes = (m_microseconds / 60'000'000) % 60;
    int seconds = (m_microseconds / 1'000'000) % 60;
    int miliseconds = (m_microseconds / 1000) % 1000;

    return QString("%1:%2:%3:%4")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'))
            .arg(miliseconds, 3, 10, QLatin1Char('0'));
}

void Time::fromMilliseconds(qint64 ms)
{
    m_microseconds = ms*1000;
}

void Time::fromMicroseconds(qint64 mc)
{
    m_microseconds = mc;
}
