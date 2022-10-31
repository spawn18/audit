#include "Wave.hpp"
#include "Utils.hpp"

#include <cstring>
#include <cmath>
#include <iostream>

#include <QMessageBox>
#include <QFileInfo>
#include <QtCore>

void Wave::load(const QString& name)
{
    m_samples.clear();

    m_path = name;
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Error: unable to open file");

    char chunkID[4];
    quint32 chunkSize;
    char wave[4];

    file.read(chunkID, 4);
    file.read(reinterpret_cast<char*>(&chunkSize), 4);
    file.read(wave, 4);

    if(strncmp(chunkID, "RIFF", 4) != 0 || strncmp(wave, "WAVE", 4) != 0)
        throw std::runtime_error("Error: incorrect file header");

    char subchunkID[4] = {0};
    quint32 subchunkSize = 0;

    bool hasFmt = false;
    bool hasData = false;

    while(!file.atEnd())
    {
        file.read(subchunkID, 4);
        file.read(reinterpret_cast<char*>(&subchunkSize), 4);

        if (std::strncmp(subchunkID, "fmt ", 4) == 0)
        {
            hasFmt = true;

            file.read(reinterpret_cast<char*>(&m_audioFormat), 2);
            file.read(reinterpret_cast<char*>(&m_numChannels), 2);
            file.read(reinterpret_cast<char*>(&m_sampleRate), 4);
            file.read(reinterpret_cast<char*>(&m_byteRate), 4);
            file.read(reinterpret_cast<char*>(&m_blockAlign), 2);
            file.read(reinterpret_cast<char*>(&m_bitsPerSample), 2);

            if(m_numChannels > 1)
                throw std::runtime_error("Error: only mono is supported");

            if(m_audioFormat != WAVE_FORMAT_PCM)
                throw std::runtime_error("Error: only PCM is supported");

            if(m_bitsPerSample != 16)
                throw std::runtime_error("Error: only sample size of 16 is supported");

        }
        else if (std::strncmp(subchunkID, "data", 4) == 0)
        {
            hasData = true;
            m_dataPos = file.pos();

            int totalSamples = subchunkSize/m_blockAlign;
            m_samples.reserve(totalSamples);

            for(int i=0; i < totalSamples; i++)
            {
                qint16 sample = 0;
                file.read(reinterpret_cast<char*>(&sample), m_blockAlign);
                m_samples.push_back(sample);
            }
        }
        else
        {
            file.skip(subchunkSize);
        }
    }

    if(!hasFmt || !hasData)
        throw std::runtime_error("Error: file is corrupted");

    if(m_samples.empty())
        throw std::runtime_error("Error: no samples were read");

    file.close();
}

qint64 Wave::readData(char *data, qint64 len)
{
    qint64 diff = 2*m_samples.size() - QIODevice::pos();
    qint64 n = len<=diff ? len : diff;
    if(diff > 0)
    {
        std::memcpy((void*)data, (void*)((char*)(m_samples.data()) + QIODevice::pos()), n);
        return n;
    }
    else return 0;
}

qint64 Wave::writeData(const char *data, qint64 len)
{
    qint64 diff = 2*m_samples.size() - QIODevice::pos();
    qint64 n = len<=diff ? len : diff;
    if(diff > 0)
    {
        std::memcpy((void*)((char*)(m_samples.data()) + QIODevice::pos()), data, n);
        return n;
    }
    else return 0;
}

Time Wave::duration() const
{
    qint64 mc = ((m_samples.size() * (m_bitsPerSample/8)) / ((long double)m_byteRate)) * 1'000'000;
    return microseconds(mc);
}

qint16& Wave::operator[](const quint64 i)
{
    return m_samples.at(i);
}

qint16 const& Wave::operator[](const quint64 i) const
{
    return m_samples.at(i);
}

qint64 Wave::totalSamples() const
{
    return m_samples.size();
}

int Wave::byteRate() const
{
    return m_byteRate;
}

int Wave::sampleRate() const
{
    return m_sampleRate;
}

int Wave::sampleSize() const
{
    return m_bitsPerSample;
}

int Wave::channels() const
{
    return m_numChannels;
}

void Wave::save()
{
    QFile file(m_path);
    file.open(QIODevice::WriteOnly);

    file.seek(m_dataPos);
    file.skip(8);
    file.write((char*)m_samples.data(), 2*m_samples.size());

    file.close();
}
