#ifndef TIME_HPP
#define TIME_HPP

#include <QtGlobal>




class Time
{
public:

     explicit Time(qint64);

    Time() = default;
    ~Time() = default;

    friend Time operator+(const Time&, const Time&);
    friend Time operator-(const Time&, const Time&);
    friend Time operator*(const Time&, double);
    friend Time operator*(double, const Time&);
    friend Time operator/(const Time&, double);
    friend Time operator/(double, const Time&);

    void operator+=(const Time&);
    void operator-=(const Time&);
    void operator/=(double);
    void operator*=(double);

    friend bool operator==(const Time&, const Time&);
    friend bool operator!=(const Time&, const Time&);
    friend bool operator<(const Time&, const Time&);
    friend bool operator>(const Time&, const Time&);
    friend bool operator<=(const Time&, const Time&);
    friend bool operator>=(const Time&, const Time&);

    void setTime(qint32 hours, qint32 minutes, qint32 seconds, qint32 ms, qint32 mcs);

    void clear();
    qint64 asMicroseconds() const;
    qint32 asMiliseconds() const;

    void setHours(qint32);
    void setMinutes(qint32);
    void setSeconds(qint32);
    void setMilliseconds(qint32);
    void setMicroseconds(qint32);

    qint32 getHours() const;
    qint32 getMinutes() const;
    qint32 getSeconds() const;
    qint32 getMilliseconds() const;
    qint32 getMicroseconds() const;

    QString asString() const;

private:

    qint64 m_microseconds{0};
};

Time hours(qint32);
Time minutes(qint32);
Time seconds(qint32);
Time milliseconds(qint64);
Time microseconds(qint64);

#endif
