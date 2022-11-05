#include "Audio.hpp"

Audio::~Audio()
{
    delete m_output;
}

void Audio::setSource(Wave *file)
{
    m_file = file;

    QAudioFormat format;
    format.setSampleRate(m_file->sampleRate());
    format.setChannelCount(m_file->channels());
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if(!info.isFormatSupported(format))
        throw std::runtime_error("Error: audio format unsupported by the device");

    delete m_output;

    m_output = new QAudioOutput(format);
    m_output->setVolume(m_volume);
    m_output->setNotifyInterval(1);

    connect(m_output, SIGNAL(notify()), this, SIGNAL(played()));
    connect(m_output, &QAudioOutput::stateChanged, [=](QAudio::State state)
    {
        if(state == QAudio::IdleState)
        {
            stop();
            emit ended();
        }
        else if(state == QAudio::StoppedState)
        {
            if(m_output->error() != QAudio::NoError)
                throw std::runtime_error("Error: audio error has occured");
        }
    });
}

void Audio::play()
{
    if(m_output->state() == QAudio::StoppedState)
        m_output->start(m_file);
}

void Audio::pause()
{
    m_output->reset();
}

void Audio::stop()
{
    m_output->reset();
    m_file->seek(0);
}

void Audio::setVolume(int value)
{
    m_output->setVolume(value/100.0);
}

bool Audio::isPlaying()
{
    return m_output->state() == QAudio::ActiveState;
}

void Audio::setTime(const Time &time)
{
    qint64 sample = (static_cast<double>(time.asMicroseconds()) / m_file->duration().asMicroseconds()) * (m_file->totalSamples()-1);
    if(sample < m_file->totalSamples())
        m_file->seek(2*sample);
}

Time Audio::getTime() const
{
    qint64 sample = m_file->pos() / 2.0;
    double ratio = sample / static_cast<double>(m_file->totalSamples()-1);
    qint64 mc = round(ratio * m_file->duration().asMicroseconds());
    return Time(mc);
}

