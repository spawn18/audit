#ifndef TIME_HPP
#define TIME_HPP

#include <QtGlobal>

class Time
{
public:

    explicit Time(qint64);

    Time() = default;
    ~Time() = default;

    void fromMilliseconds(qint64);
    void fromMicroseconds(qint64);

    qint64 asMicroseconds() const;

    QString asString() const;

private:

    qint64 m_microseconds{0};
};

#endif
