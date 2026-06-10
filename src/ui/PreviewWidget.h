#pragma once
#include "core/Document.h"
#include "engine/AudioEngine.h"
#include "engine/RenderWorker.h"
#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

class QComboBox;
class QLabel;
class QTabBar;
class QToolButton;
class VideoArea;

// Source/Program monitor with transport, preview quality selection and
// direct-manipulation transform handles for the selected clip.
class PreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit PreviewWidget(Document *doc, QWidget *parent = nullptr);

    void previewMedia(const QString &mediaId);  // switch to Source monitor
    void playPause();
    void stop();
    void stepFrames(int frames);
    void goToStart();
    void goToEnd();
    bool isPlaying() const { return m_playing; }
    double renderScale() const;
    QString monitoredSequence() const;

private:
    friend class VideoArea;
    void tick();
    void requestRender();
    void setPlaying(bool on);
    Sequence *monitoredSeq() const;
    double playhead() const;

    Document *m_doc;
    RenderWorker *m_worker;
    AudioEngine *m_audio;
    QTimer m_timer;
    QElapsedTimer m_wallClock;  // fallback when no audio device exists
    double m_wallStart = 0;
    bool m_playing = false;
    bool m_programMode = true;
    QString m_sourceSeqId;

    VideoArea *m_video;
    QTabBar *m_tabs;
    QLabel *m_timecode;
    QComboBox *m_quality;
    QToolButton *m_playBtn;
};

// The actual image surface + interactive overlay.
class VideoArea : public QWidget {
    Q_OBJECT
public:
    explicit VideoArea(PreviewWidget *owner, Document *doc);
    void setFrame(const QImage &img, double t);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;

private:
    enum class DragMode { None, Move, Scale };
    QRectF frameRect() const;  // where the frame is drawn (letterboxed)
    // selected clip geometry in widget coords (ignores rotation for hits)
    bool selectedClipRect(QRectF &out, Clip **clip, Sequence **seq) const;
    Clip *topClipAt(const QPointF &pos) const;  // topmost video clip under pos

    PreviewWidget *m_owner;
    Document *m_doc;
    QImage m_frame;
    double m_frameT = 0;
    DragMode m_drag = DragMode::None;
    QPointF m_dragStart;
    int m_corner = 0;
    double m_startPosX = 0, m_startPosY = 0, m_startSX = 1, m_startSY = 1;
    QRectF m_startRect;
    bool m_undoStarted = false;
};
