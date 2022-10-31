#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <QtMultimedia/QAudioOutput>

#include "Time.hpp"
#include "Wave.hpp"

class Audio : public QObject
{
    Q_OBJECT

public:

    explicit Audio(Wave* file, QObject *parent = nullptr);

    Audio() = default;
    ~Audio();

    void setSource(Wave* file);

    void play();
    void pause();
    void stop();

    bool isPlaying();

    void setTime(const Time&);
    Time getTime() const;


public slots:
    void setVolume(int);
    void handleStateChange(QAudio::State);

signals:
    void played();
    void ended();

private:

    Wave* m_file{nullptr};
    QAudioOutput *m_output{nullptr};

    double m_volume{0.5};
};

#endif
