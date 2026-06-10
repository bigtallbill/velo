#include "ui/PreviewWidget.h"
#include "engine/Compositor.h"
#include "ui/Theme.h"
#include <QComboBox>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QTabBar>
#include <QToolButton>
#include <QVBoxLayout>

PreviewWidget::PreviewWidget(Document *doc, QWidget *parent)
    : QWidget(parent), m_doc(doc) {
    m_worker = new RenderWorker(&doc->project(), doc->mutex(), this);
    m_audio = new AudioEngine(&doc->project(), doc->mutex(), this);
    m_worker->start();

    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    m_tabs = new QTabBar;
    m_tabs->addTab(tr("Source"));
    m_tabs->addTab(tr("Program"));
    m_tabs->setCurrentIndex(1);
    m_tabs->setExpanding(false);
    m_tabs->setDrawBase(false);
    lay->addWidget(m_tabs);

    m_video = new VideoArea(this, doc);
    lay->addWidget(m_video, 1);

    auto *bar = new QHBoxLayout;
    bar->setContentsMargins(6, 3, 6, 3);
    auto mkBtn = [&](const QString &text, const QString &tip) {
        auto *b = new QToolButton;
        b->setText(text);
        b->setToolTip(tip);
        b->setAutoRaise(true);
        bar->addWidget(b);
        return b;
    };
    m_timecode = new QLabel("00:00:00:00");
    m_timecode->setStyleSheet("font-family: monospace; color: #4f9cf5;");
    bar->addWidget(m_timecode);
    bar->addStretch(1);
    auto *startBtn = mkBtn("⏮", tr("Go to start (Home)"));
    auto *backBtn = mkBtn("◀▮", tr("Step one frame back (Left)"));
    m_playBtn = mkBtn("▶", tr("Play / Pause (Space)"));
    auto *fwdBtn = mkBtn("▮▶", tr("Step one frame forward (Right)"));
    auto *endBtn = mkBtn("⏭", tr("Go to end (End)"));
    bar->addStretch(1);
    m_quality = new QComboBox;
    m_quality->addItems({tr("Full"), tr("1/2"), tr("1/4"), tr("1/8")});
    m_quality->setCurrentIndex(1);
    m_quality->setToolTip(tr("Preview render quality"));
    bar->addWidget(new QLabel(tr("Quality:")));
    bar->addWidget(m_quality);
    lay->addLayout(bar);

    connect(startBtn, &QToolButton::clicked, this, &PreviewWidget::goToStart);
    connect(backBtn, &QToolButton::clicked, this, [this] { stepFrames(-1); });
    connect(m_playBtn, &QToolButton::clicked, this, &PreviewWidget::playPause);
    connect(fwdBtn, &QToolButton::clicked, this, [this] { stepFrames(1); });
    connect(endBtn, &QToolButton::clicked, this, &PreviewWidget::goToEnd);
    connect(m_quality, &QComboBox::currentIndexChanged, this,
            [this] { requestRender(); });
    connect(m_tabs, &QTabBar::currentChanged, this, [this](int idx) {
        setPlaying(false);
        m_programMode = idx == 1;
        requestRender();
    });

    connect(m_worker, &RenderWorker::frameReady, this,
            [this](const QImage &img, const QString &seqId, double t) {
                if (seqId == monitoredSequence()) m_video->setFrame(img, t);
            });
    connect(doc, &Document::playheadChanged, this,
            [this](const QString &seqId, double t) {
                if (seqId != monitoredSequence()) return;
                m_timecode->setText(formatTimecode(
                    t, monitoredSeq() ? monitoredSeq()->fps : 30.0));
                if (!m_playing) requestRender();
            });
    connect(doc, &Document::sequenceChanged, this, [this](const QString &seqId) {
        if (seqId == monitoredSequence() && !m_playing) requestRender();
    });
    connect(doc, &Document::activeSequenceChanged, this, [this] {
        if (m_programMode) {
            setPlaying(false);
            requestRender();
        }
    });
    connect(doc, &Document::selectionChanged, this,
            [this] { m_video->update(); });
    connect(doc, &Document::projectLoaded, this, [this] {
        setPlaying(false);
        m_sourceSeqId.clear();
        requestRender();
    });

    m_timer.setInterval(15);
    connect(&m_timer, &QTimer::timeout, this, &PreviewWidget::tick);
}

QString PreviewWidget::monitoredSequence() const {
    return m_programMode ? m_doc->project().activeSequence : m_sourceSeqId;
}

Sequence *PreviewWidget::monitoredSeq() const {
    return m_doc->project().sequenceById(monitoredSequence());
}

double PreviewWidget::playhead() const {
    return m_doc->playhead(monitoredSequence());
}

double PreviewWidget::renderScale() const {
    switch (m_quality->currentIndex()) {
    case 0: return 1.0;
    case 2: return 0.25;
    case 3: return 0.125;
    default: return 0.5;
    }
}

void PreviewWidget::previewMedia(const QString &mediaId) {
    setPlaying(false);
    m_sourceSeqId = m_doc->ensureSourceSequence(mediaId);
    m_doc->setPlayhead(m_sourceSeqId, 0);
    m_tabs->setCurrentIndex(0);
    m_programMode = false;
    requestRender();
}

void PreviewWidget::requestRender() {
    const QString seqId = monitoredSequence();
    if (seqId.isEmpty()) {
        m_video->setFrame(QImage(), 0);
        return;
    }
    m_worker->requestFrame(seqId, playhead(), renderScale());
}

void PreviewWidget::setPlaying(bool on) {
    if (m_playing == on) return;
    m_playing = on;
    m_playBtn->setText(on ? "⏸" : "▶");
    if (on) {
        const double t = playhead();
        m_audio->invalidateReaders();
        m_audio->play(monitoredSequence(), t);
        m_wallClock.start();
        m_wallStart = t;
        m_timer.start();
    } else {
        m_audio->stop();
        m_timer.stop();
        requestRender();  // settle on an exact frame
    }
}

void PreviewWidget::playPause() {
    if (monitoredSequence().isEmpty()) return;
    setPlaying(!m_playing);
}

void PreviewWidget::stop() { setPlaying(false); }

void PreviewWidget::tick() {
    Sequence *seq = monitoredSeq();
    if (!seq) {
        setPlaying(false);
        return;
    }
    double t = m_audio->clock();
    if (t <= m_wallStart + 1e-6)  // no audio device: wall clock fallback
        t = m_wallStart + m_wallClock.elapsed() / 1000.0;
    const double dur = seq->duration();
    if (dur > 0 && t >= dur) {
        m_doc->setPlayhead(seq->id, dur);
        setPlaying(false);
        return;
    }
    m_doc->setPlayhead(seq->id, t);
    if (!m_worker->busy())
        m_worker->requestFrame(seq->id, t, renderScale());
}

void PreviewWidget::stepFrames(int frames) {
    Sequence *seq = monitoredSeq();
    if (!seq) return;
    setPlaying(false);
    const double fd = seq->frameDur();
    double t = seq->snapFrame(playhead()) + frames * fd;
    m_doc->setPlayhead(seq->id, qMax(0.0, t));
}

void PreviewWidget::goToStart() {
    if (Sequence *seq = monitoredSeq()) {
        setPlaying(false);
        m_doc->setPlayhead(seq->id, 0);
    }
}

void PreviewWidget::goToEnd() {
    if (Sequence *seq = monitoredSeq()) {
        setPlaying(false);
        m_doc->setPlayhead(seq->id, seq->duration());
    }
}

// ------------------------------------------------------------------ VideoArea
VideoArea::VideoArea(PreviewWidget *owner, Document *doc)
    : QWidget(owner), m_owner(owner), m_doc(doc) {
    setMinimumSize(320, 180);
    setMouseTracking(true);
    setAutoFillBackground(false);
}

void VideoArea::setFrame(const QImage &img, double t) {
    m_frame = img;
    m_frameT = t;
    update();
}

QRectF VideoArea::frameRect() const {
    if (m_frame.isNull()) return {};
    QSizeF s = QSizeF(m_frame.size());
    s.scale(size(), Qt::KeepAspectRatio);
    return QRectF(QPointF((width() - s.width()) / 2, (height() - s.height()) / 2),
                  s);
}

bool VideoArea::selectedClipRect(QRectF &out, Clip **clipOut,
                                 Sequence **seqOut) const {
    if (!m_owner->m_programMode) return false;
    Sequence *seq = m_owner->monitoredSeq();
    if (!seq) return false;
    const quint64 id = m_doc->soloSelectedClip();
    if (!id) return false;
    Clip *clip = seq->findClip(id);
    if (!clip || !clip->isVideoKind()) return false;
    const double t = m_doc->playhead(seq->id);
    if (t < clip->start || t >= clip->end()) return false;
    const QRectF fr = frameRect();
    if (fr.isEmpty()) return false;
    const double k = fr.width() / seq->width;  // sequence px -> widget px
    const double local = clip->clipLocal(t);
    QSizeF nat = Compositor::clipNativeSize(m_doc->project(), *seq, *clip);
    const double sx = clip->scaleX.at(local);
    const double sy = clip->uniformScale ? sx : clip->scaleY.at(local);
    const double w = nat.width() * sx * k, h = nat.height() * sy * k;
    const QPointF c(fr.left() + (seq->width / 2.0 + clip->posX.at(local)) * k,
                    fr.top() + (seq->height / 2.0 + clip->posY.at(local)) * k);
    out = QRectF(c.x() - w / 2, c.y() - h / 2, w, h);
    if (clipOut) *clipOut = clip;
    if (seqOut) *seqOut = seq;
    return true;
}

void VideoArea::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.fillRect(rect(), QColor(12, 12, 14));
    const QRectF fr = frameRect();
    if (!m_frame.isNull()) {
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        p.drawImage(fr, m_frame);
    } else {
        p.setPen(Theme::textDim());
        p.drawText(rect(), Qt::AlignCenter,
                   tr("Drop media into the timeline, or double-click a clip\n"
                      "in the project panel to preview it here"));
    }
    QRectF sel;
    Clip *clip;
    Sequence *seq;
    if (selectedClipRect(sel, &clip, &seq)) {
        p.setRenderHint(QPainter::Antialiasing);
        const double local = clip->clipLocal(m_doc->playhead(seq->id));
        p.save();
        p.translate(sel.center());
        p.rotate(clip->rotation.at(local));
        p.translate(-sel.center());
        p.setPen(QPen(Theme::accent(), 1.5));
        p.setBrush(Qt::NoBrush);
        p.drawRect(sel);
        p.setBrush(Theme::accent());
        for (const QPointF &c : {sel.topLeft(), sel.topRight(),
                                 sel.bottomLeft(), sel.bottomRight()})
            p.drawRect(QRectF(c.x() - 4, c.y() - 4, 8, 8));
        p.restore();
    }
}

void VideoArea::mousePressEvent(QMouseEvent *e) {
    if (e->button() != Qt::LeftButton) return;
    QRectF sel;
    Clip *clip;
    Sequence *seq;
    m_drag = DragMode::None;
    m_undoStarted = false;
    if (selectedClipRect(sel, &clip, &seq)) {
        const QPointF corners[4] = {sel.topLeft(), sel.topRight(),
                                    sel.bottomLeft(), sel.bottomRight()};
        for (int i = 0; i < 4; ++i) {
            if (QLineF(e->position(), corners[i]).length() < 9) {
                m_drag = DragMode::Scale;
                m_corner = i;
                break;
            }
        }
        if (m_drag == DragMode::None && sel.contains(e->position()))
            m_drag = DragMode::Move;
        if (m_drag != DragMode::None) {
            const double local = clip->clipLocal(m_doc->playhead(seq->id));
            m_dragStart = e->position();
            m_startRect = sel;
            m_startPosX = clip->posX.at(local);
            m_startPosY = clip->posY.at(local);
            m_startSX = clip->scaleX.at(local);
            m_startSY = clip->uniformScale ? m_startSX : clip->scaleY.at(local);
            return;
        }
    }
    // click-select the topmost video clip under the cursor
    Sequence *aseq = m_owner->monitoredSeq();
    const QRectF fr = frameRect();
    if (m_owner->m_programMode && aseq && !fr.isEmpty()) {
        const double t = m_doc->playhead(aseq->id);
        const double k = fr.width() / aseq->width;
        for (int i = aseq->videoTracks.size() - 1; i >= 0; --i) {
            if (aseq->videoTracks[i].muted) continue;
            Clip *c = aseq->videoTracks[i].clipAt(t);
            if (!c) continue;
            const double local = c->clipLocal(t);
            QSizeF nat = Compositor::clipNativeSize(m_doc->project(), *aseq, *c);
            const double sx = c->scaleX.at(local);
            const double sy = c->uniformScale ? sx : c->scaleY.at(local);
            const QPointF ctr(
                fr.left() + (aseq->width / 2.0 + c->posX.at(local)) * k,
                fr.top() + (aseq->height / 2.0 + c->posY.at(local)) * k);
            QRectF r(ctr.x() - nat.width() * sx * k / 2,
                     ctr.y() - nat.height() * sy * k / 2, nat.width() * sx * k,
                     nat.height() * sy * k);
            if (r.contains(e->position())) {
                m_doc->setSelectedClips({c->id});
                return;
            }
        }
        m_doc->clearSelection();
    }
}

void VideoArea::mouseMoveEvent(QMouseEvent *e) {
    QRectF sel;
    Clip *clip;
    Sequence *seq;
    if (m_drag == DragMode::None) {
        if (selectedClipRect(sel, &clip, &seq)) {
            bool corner = false;
            for (const QPointF &c : {sel.topLeft(), sel.topRight(),
                                     sel.bottomLeft(), sel.bottomRight()})
                if (QLineF(e->position(), c).length() < 9) corner = true;
            setCursor(corner ? Qt::SizeFDiagCursor
                             : (sel.contains(e->position()) ? Qt::SizeAllCursor
                                                            : Qt::ArrowCursor));
        } else {
            setCursor(Qt::ArrowCursor);
        }
        return;
    }
    if (!selectedClipRect(sel, &clip, &seq)) return;
    if (!m_undoStarted) {
        m_doc->beginUndoStep();
        m_undoStarted = true;
    }
    const QRectF fr = frameRect();
    const double k = fr.width() / seq->width;
    const double local = clip->clipLocal(m_doc->playhead(seq->id));
    const QPointF d = e->position() - m_dragStart;

    if (m_drag == DragMode::Move) {
        clip->posX.setAt(local, m_startPosX + d.x() / k);
        clip->posY.setAt(local, m_startPosY + d.y() / k);
    } else {
        const QPointF center = m_startRect.center();
        const QPointF startCorner = (m_corner == 0)   ? m_startRect.topLeft()
                                    : (m_corner == 1) ? m_startRect.topRight()
                                    : (m_corner == 2) ? m_startRect.bottomLeft()
                                                      : m_startRect.bottomRight();
        const QPointF now = startCorner + d;
        if (e->modifiers() & Qt::ShiftModifier) {
            // distort: scale x and y independently
            clip->uniformScale = false;
            const double fx = std::abs(now.x() - center.x()) /
                              qMax(1.0, std::abs(startCorner.x() - center.x()));
            const double fy = std::abs(now.y() - center.y()) /
                              qMax(1.0, std::abs(startCorner.y() - center.y()));
            clip->scaleX.setAt(local, qMax(0.01, m_startSX * fx));
            clip->scaleY.setAt(local, qMax(0.01, m_startSY * fy));
        } else {
            const double f = QLineF(center, now).length() /
                             qMax(1.0, QLineF(center, startCorner).length());
            clip->scaleX.setAt(local, qMax(0.01, m_startSX * f));
            if (!clip->uniformScale)
                clip->scaleY.setAt(local, qMax(0.01, m_startSY * f));
        }
    }
    m_doc->notifySequenceChanged(seq->id);
}

void VideoArea::mouseReleaseEvent(QMouseEvent *) {
    if (m_drag != DragMode::None && m_undoStarted)
        emit m_doc->selectionChanged();  // sync spinboxes in Effect Controls
    m_drag = DragMode::None;
}
