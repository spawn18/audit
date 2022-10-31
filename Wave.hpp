#ifndef WAVE_HPP
#define WAVE_HPP

#include <QtGlobal>
#include <QtCore>
#include <QIODevice>
#include <QFile>
#include <QString>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

#include "Time.hpp"

#include <vector>

class Wave : public QIODevice
{
    Q_OBJECT

public:

    Wave() = default;
   ~Wave() = default;

    void load(const QString&);
    void save();

    int byteRate() const;
    int sampleRate() const;
    int sampleSize() const;
    int channels() const;
    Time duration() const;
    qint64 totalSamples() const;

    qint16& operator[](const quint64);
    qint16 const& operator[](const quint64) const;

protected:

    virtual qint64 readData(char *data, qint64 len);
    virtual qint64 writeData(const char *data, qint64 len);

private:

    QString m_path;

    quint32 m_chunkSize{0};
    quint16 m_audioFormat{0};
    quint16 m_numChannels{0};
    quint32 m_sampleRate{0};
    quint32 m_byteRate{0};
    quint16 m_blockAlign{0};
    quint16 m_bitsPerSample{0};

    qint64 m_dataPos{0};
    std::vector<qint16> m_samples;
};

#endif
