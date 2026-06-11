#pragma once
#include "core/Model.h"
#include <QImage>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

// Background preview renderer. Requests coalesce: only the most recent
// (sequence, time, scale) is rendered, so scrubbing and playback never
// queue up stale frames. In adaptive mode (playback) the resolution is
// temporarily reduced when frames take longer than the frame budget, so
// heavy sections (dissolves between nests, effect stacks) play smoothly
// instead of stuttering; paused frames always render at full quality.
class RenderWorker : public QThread {
    Q_OBJECT
public:
    RenderWorker(Project *project, QRecursiveMutex *modelMutex,
                 QObject *parent = nullptr);
    ~RenderWorker() override;

    void requestFrame(const QString &seqId, double t, double scale);
    bool busy() const { return m_busy.loadAcquire(); }
    // Playback on/off: enables the smoothness-over-resolution trade.
    void setAdaptive(bool on) { m_adaptive.storeRelease(on ? 1 : 0); }

signals:
    void frameReady(const QImage &img, const QString &seqId, double t);

protected:
    void run() override;

private:
    Project *m_project;
    QRecursiveMutex *m_modelMutex;
    QMutex m_mutex;
    QWaitCondition m_cond;
    QString m_seqId;
    double m_t = 0, m_scale = 0.5;
    bool m_hasRequest = false;
    bool m_quit = false;
    QAtomicInt m_busy{0};
    QAtomicInt m_adaptive{0};
};
