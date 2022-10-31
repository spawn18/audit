#ifndef TRACK_HPP
#define TRACK_HPP

#include "Wave.hpp"
#include "Audio.hpp"
#include "Selection.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QWidget>

class Track : public QWidget
{
    Q_OBJECT

public:
    explicit    Track(QWidget *parent = nullptr);
    virtual     ~Track() = default;

    bool        isSelected();
    void        updateSelection(const Selection&, const Wave&);

    void        setSelection(const Selection&);
    Selection   getSelection();

    void        setPosition(double);
    double      getPosition();

signals:
    void        clicked(double);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:


    int       m_cursor{-1};
    double    m_position{0};

    Selection m_selection;

    std::vector<std::pair<int,int>> m_waveformMean;
    std::vector<std::pair<int,int>> m_waveformMax;
};


#endif
