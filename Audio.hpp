#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <QtMultimedia/QAudioOutput>

#include "Time.hpp"
#include "Wave.hpp"

class Audio : public QObject
{
    Q_OBJECT

public:
     Audio() = default;
    ~Audio();

    void setSource(Wave* file);

    void play();
    void pause();
    void stop();

    void setTime(const Time&);
    Time getTime() const;

public slots:
    void setVolume(int);

signals:
    void played();
    void ended();

private:

    Wave* m_file{nullptr};
    QAudioOutput *m_output{nullptr};

    double m_volume{0.5};
};

#endif
