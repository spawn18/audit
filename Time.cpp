#include "Time.hpp"

#include <QString>

Time::Time(qint64 mcs)
{
    m_microseconds = mcs;
}

void Time::setTime(qint32 hours, qint32 minutes, qint32 seconds, qint32 ms, qint32 mcs)
{
    this->m_microseconds = hours*3'600'000'000 + minutes*60'000'000 + seconds*1'000'000 + ms*1000 + mcs;
}

void Time::clear()
{
    m_microseconds = 0;
}

Time operator+(const Time &t1, const Time &t2)
{
    return Time(t1.m_microseconds + t2.m_microseconds);
}

Time operator-(const Time &t1, const Time &t2)
{
    return Time(t1.m_microseconds - t2.m_microseconds);
}

Time operator*(const Time &time, double t)
{
    return Time(static_cast<qint64>(time.m_microseconds * t));
}

Time operator*(double t, const Time &time)
{
    return time*t;
}

Time operator/(const Time &time, double t)
{
    return Time(time.m_microseconds / t);
}

Time operator/(double t, const Time &time)
{
    return time/t;
}

bool operator==(const Time& t1, const Time& t2)
{
    return t1.m_microseconds == t2.m_microseconds;
}

bool operator!=(const Time& t1, const Time& t2)
{
    return t1.m_microseconds != t2.m_microseconds;
}

bool operator<(const Time& t1, const Time& t2)
{
    return t1.m_microseconds < t2.m_microseconds;
}

bool operator>(const Time& t1, const Time& t2)
{
    return t1.m_microseconds > t2.m_microseconds;
}

bool operator<=(const Time& t1, const Time& t2)
{
    return t1.m_microseconds <= t2.m_microseconds;
}

bool operator>=(const Time& t1, const Time& t2)
{
    return t1.m_microseconds >= t2.m_microseconds;
}

void Time::operator+=(const Time& time)
{
    m_microseconds += time.m_microseconds;
}

void Time::operator-=(const Time& time)
{
    m_microseconds -= time.m_microseconds;
}

void Time::operator*=(double t)
{
    m_microseconds *= t;
}

void Time::operator/=(double t)
{
    m_microseconds /= t;
}

qint32 Time::getHours() const
{
    return (m_microseconds / 3'600'000'000) % 60;
}

qint32 Time::getMinutes() const
{
    return (m_microseconds / 60'000'000) % 60;
}

qint32 Time::getSeconds() const
{
    return (m_microseconds / 1'000'000) % 60;
}

qint32 Time::getMilliseconds() const
{
    return (m_microseconds / 1000) % 1000;
}

qint32 Time::getMicroseconds() const
{
    return m_microseconds % 1000;
}

void Time::setHours(qint32 value)
{
    m_microseconds -= this->getHours() * 3'600'000'000;
    m_microseconds += value * 3'600'000'000;
}

void Time::setMinutes(qint32 value)
{
    m_microseconds -= this->getMinutes() * 60'000'000;
    m_microseconds += value * 60'000'000;
}

void Time::setSeconds(qint32 value)
{
    m_microseconds -= this->getSeconds() * 1'000'000;
    m_microseconds += value * 1'000'000;
}

void Time::setMilliseconds(qint32 value)
{
    m_microseconds -= this->getMilliseconds() * 1000;
    m_microseconds += value * 1000;
}

void Time::setMicroseconds(qint32 value)
{
    m_microseconds -= getMicroseconds();
    m_microseconds += value;
}

qint64 Time::asMicroseconds() const
{
    return m_microseconds;
}

qint32 Time::asMiliseconds() const
{
    return m_microseconds/1000;
}

QString Time::asString() const
{
    return QString("%1:%2:%3:%4")
            .arg(getHours(), 2, 10, QLatin1Char('0'))
            .arg(getMinutes(), 2, 10, QLatin1Char('0'))
            .arg(getSeconds(), 2, 10, QLatin1Char('0'))
            .arg(getMilliseconds(), 3, 10, QLatin1Char('0'));
}

Time hours(qint32 value)
{
    return Time(value*3'600'000'000);
}

Time minutes(qint32 value)
{
    return Time(value*60'000'000);
}

Time seconds(qint32 value)
{
    return Time(value*1'000'000);
}

Time milliseconds(qint64 value)
{
    return Time(value*1000);
}

Time microseconds(qint64 value)
{
    return Time(value);
}
