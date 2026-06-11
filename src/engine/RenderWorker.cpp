#include "engine/RenderWorker.h"
#include "engine/Compositor.h"
#include <QElapsedTimer>

RenderWorker::RenderWorker(Project *project, QRecursiveMutex *modelMutex,
                           QObject *parent)
    : QThread(parent), m_project(project), m_modelMutex(modelMutex) {}

RenderWorker::~RenderWorker() {
    {
        QMutexLocker lock(&m_mutex);
        m_quit = true;
        m_cond.wakeAll();
    }
    wait(3000);
}

void RenderWorker::requestFrame(const QString &seqId, double t, double scale) {
    QMutexLocker lock(&m_mutex);
    m_seqId = seqId;
    m_t = t;
    m_scale = scale;
    m_hasRequest = true;
    m_cond.wakeAll();
}

void RenderWorker::run() {
    Compositor comp(m_project, m_modelMutex);
    double autoScale = 1.0;  // adaptive degrade factor, worker-thread only
    int fastStreak = 0;
    QElapsedTimer timer;
    while (true) {
        QString seqId;
        double t, scale;
        {
            QMutexLocker lock(&m_mutex);
            while (!m_hasRequest && !m_quit) m_cond.wait(&m_mutex);
            if (m_quit) return;
            seqId = m_seqId;
            t = m_t;
            scale = m_scale;
            m_hasRequest = false;
        }
        const bool adaptive = m_adaptive.loadAcquire();
        if (!adaptive) {
            autoScale = 1.0;
            fastStreak = 0;
        }
        m_busy.storeRelease(1);
        timer.start();
        QImage img = comp.renderFrame(seqId, t, qMax(0.1, scale * autoScale));
        const qint64 ms = timer.elapsed();
        m_busy.storeRelease(0);
        if (adaptive) {
            if (ms > 36 && autoScale > 0.26) {
                autoScale *= 0.5;  // missing the ~30 fps budget: drop quality
                fastStreak = 0;
            } else if (ms < 9 && autoScale < 0.99) {
                // plenty of headroom for a while: restore quality gradually
                if (++fastStreak >= 30) {
                    autoScale = qMin(1.0, autoScale * 2.0);
                    fastStreak = 0;
                }
            } else {
                fastStreak = 0;
            }
        }
        if (!img.isNull()) emit frameReady(img, seqId, t);
    }
}
