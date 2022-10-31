#include "Track.hpp"
#include "Utils.hpp"

#include <QMessageBox>

Track::Track(QWidget *parent) : QWidget(parent)
{
}

void Track::updateSelection(const Selection& selection, const Wave& file)
{
    m_waveformMax.resize(width());
    m_waveformMean.resize(width());

    int samplesPerPixel = file.totalSamples() / static_cast<double>(width());

    int x1 = selection.begin * (width()-1);
    int x2 = selection.end * (width()-1);

    for(int i = x1; i <= x2; i++)
    {
        double meanNumerator = 0.0;
        double maxSample = 0.0;
        double minSample = 0.0;

        for(int j = samplesPerPixel*i; j < samplesPerPixel*(i+1); j++)
        {
            double sample = file[j] / static_cast<double>(MAX_SAMPLE_VALUE);

            meanNumerator += pow(sample, 2) / samplesPerPixel;

            if(sample > maxSample)
               maxSample = sample;
            else if(sample < minSample)
               minSample = sample;
        }

        int min = height()/2.0 * (1 - minSample);
        int max = height()/2.0 * (1 - maxSample);

        int meanNeg = height()/2.0 * (1 + sqrt(meanNumerator));
        int meanPos = height()/2.0 * (1 - sqrt(meanNumerator));

        std::pair<int, int> pairMax{min, max};
        std::pair<int, int> pairMean{meanNeg, meanPos};

        m_waveformMax[i] = pairMax;
        m_waveformMean[i] = pairMean;
    }

    update();
}

bool Track::isSelected()
{
    return !m_selection.isEmpty();
}

void Track::setPosition(double pos)
{
    m_position = pos;
    update();
}

void Track::setSelection(const Selection& selection)
{
    m_selection = selection;
    update();
}

double Track::getPosition()
{
    return m_position;
}

Selection Track::getSelection()
{
    return m_selection;
}

void Track::mouseMoveEvent(QMouseEvent *event)
{
    if(this->rect().contains(event->pos()))
    {
        m_cursor = event->pos().x();

        if(event->buttons() & Qt::LeftButton)
        {
            double x = static_cast<double>(m_cursor) / (width()-1);

            if(m_position <= x)
                m_selection.end = x;
            else
                m_selection.begin = x;
        }

        update();
    }
}

void Track::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        m_position = static_cast<double>(event->pos().x()) / (width()-1);

        m_selection.begin = m_position;
        m_selection.end = m_position;

        update();
        emit clicked(m_position);
    }
}

void Track::leaveEvent(QEvent *event)
{
    m_cursor = -1;
    update();
}

void Track::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPalette palette;

    if(!isEnabled())
    {
        palette.setColor(QPalette::Window, QColor(200, 200, 200));
        setPalette(palette);
        return;
    }

    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);

    int left = event->rect().left();
    int right = left + event->rect().width();

    for(int i = left; i < right; i++)
    {
        int yMax1 = m_waveformMax[i].first;
        int yMax2 = m_waveformMax[i].second;

        painter.setPen(COLOR_WAVEFORM_MAX);
        painter.drawLine(QLine(i, yMax1, i, yMax2));

        int yMean1 = m_waveformMean[i].first;
        int yMean2 = m_waveformMean[i].second;

        painter.setPen(COLOR_WAVEFORM_MEAN);
        painter.drawLine(QLine(i, yMean1, i, yMean2));
    }

    painter.setPen(Qt::NoPen);

    if(isSelected())
    {
        painter.setBrush(QColor(225, 255, 255, 128));

        QRect rect;
        rect.setTop(0);
        rect.setHeight(height());
        rect.setLeft(m_selection.begin * (width()-1));
        rect.setRight(m_selection.end  * (width()-1));

        painter.drawRect(rect);
    }

    if(m_cursor != -1)
    {
        painter.setPen(COLOR_CURSOR);
        painter.drawLine(QLine(m_cursor, 0, m_cursor, height()));
    }

    int pos = m_position * (width()-1);
    painter.setPen(COLOR_POSITION);
    painter.drawLine(QLine(pos, 0, pos, height()));
}
