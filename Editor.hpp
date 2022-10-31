#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "Properties.hpp"
#include "Wave.hpp"
#include "Audio.hpp"
#include "Track.hpp"

#include <QMainWindow>
#include <QFile>
#include <QShortcut>
#include <QtMultimedia/QAudioOutput>


QT_BEGIN_NAMESPACE
namespace Ui { class Editor; }
QT_END_NAMESPACE


class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
            ~Editor();

private slots:

    void open();
    void save();
    void properties();

    void erase();
    void paste();
    void copy();
    void cut();

    void reverse();
    void generateTone(QAction *);
    void fadeIn();
    void fadeOut();
    void whiteNoise();

    void toggle(bool checked);
    void stop();

    void handlePlayed();
    void handleEnded();
    void handleTrackClick(double);

private:
    void enableUI(bool state);

    Wave m_file;
    Audio m_audio;

    std::vector<qint16> m_buffer;
    bool m_isBufferSet{false};

    Ui::Editor *ui;
};

#endif
