#include "ui/MediaBin.h"
#include "media/MediaCache.h"
#include "ui/Theme.h"
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QToolButton>
#include <QVBoxLayout>
#include <QtConcurrent>

static const char *kMime = "application/x-velo-item";

QMimeData *MediaListWidget::mimeData(const QList<QListWidgetItem *> &items) const {
    auto *mime = new QMimeData;
    if (!items.isEmpty())
        mime->setData(kMime, items.first()->data(Qt::UserRole).toString().toUtf8());
    return mime;
}

MediaBin::MediaBin(Document *doc, QWidget *parent) : QWidget(parent), m_doc(doc) {
    setAcceptDrops(true);
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(4, 4, 4, 4);
    lay->setSpacing(4);

    auto *bar = new QHBoxLayout;
    auto *importBtn = new QToolButton;
    importBtn->setText(tr("Import…"));
    importBtn->setToolTip(tr("Import media files (Ctrl+I)"));
    connect(importBtn, &QToolButton::clicked, this, &MediaBin::importFilesDialog);
    m_search = new QLineEdit;
    m_search->setPlaceholderText(tr("Search…"));
    m_search->setClearButtonEnabled(true);
    connect(m_search, &QLineEdit::textChanged, this, &MediaBin::refresh);
    bar->addWidget(importBtn);
    bar->addWidget(m_search, 1);
    lay->addLayout(bar);

    m_list = new MediaListWidget;
    m_list->setViewMode(QListView::IconMode);
    m_list->setIconSize(QSize(96, 54));
    m_list->setGridSize(QSize(118, 92));
    m_list->setResizeMode(QListView::Adjust);
    m_list->setWordWrap(true);
    m_list->setDragEnabled(true);
    m_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_list->setContextMenuPolicy(Qt::CustomContextMenu);
    m_list->setTextElideMode(Qt::ElideMiddle);
    lay->addWidget(m_list, 1);

    connect(m_list, &QListWidget::customContextMenuRequested, this,
            &MediaBin::contextMenu);
    connect(m_list, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *it) {
        const QString ref = it->data(Qt::UserRole).toString();
        if (ref.startsWith("sequence:"))
            m_doc->openSequenceTab(ref.mid(9));
        else if (ref.startsWith("media:"))
            emit previewRequested(ref.mid(6));
    });
    connect(m_doc, &Document::mediaChanged, this, &MediaBin::refresh);
    connect(m_doc, &Document::sequenceListChanged, this, &MediaBin::refresh);
    connect(m_doc, &Document::projectLoaded, this, [this] {
        m_thumbs.clear();
        refresh();
    });
    refresh();
}

void MediaBin::importFilesDialog() {
    const QStringList files = QFileDialog::getOpenFileNames(
        this, tr("Import Media"), QDir::homePath(),
        tr("Media files (*.mp4 *.mov *.mkv *.webm *.avi *.m4v *.mts *.mp3 *.wav "
           "*.flac *.aac *.ogg *.opus *.m4a *.png *.jpg *.jpeg *.webp *.bmp "
           "*.tif *.tiff *.gif *.svg);;All files (*)"));
    if (!files.isEmpty()) {
        m_doc->beginUndoStep();
        const QStringList ids = m_doc->importMedia(files);
        for (const QString &id : ids) makeThumbnail(id);
    }
}

static QIcon paintIcon(const QImage &frame, const QColor &fallback,
                       const QString &glyph) {
    QImage img(192, 108, QImage::Format_ARGB32_Premultiplied);
    img.fill(fallback);
    QPainter p(&img);
    if (!frame.isNull()) {
        QImage s = frame.scaled(img.size(), Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);
        p.fillRect(img.rect(), QColor(20, 20, 22));
        p.drawImage((img.width() - s.width()) / 2, (img.height() - s.height()) / 2, s);
    } else if (!glyph.isEmpty()) {
        QFont f = p.font();
        f.setPixelSize(48);
        p.setFont(f);
        p.setPen(QColor(255, 255, 255, 180));
        p.drawText(img.rect(), Qt::AlignCenter, glyph);
    }
    p.end();
    return QIcon(QPixmap::fromImage(img));
}

void MediaBin::makeThumbnail(const QString &mediaId) {
    const MediaItem *m = m_doc->project().mediaByIdConst(mediaId);
    if (!m) return;
    const MediaItem item = *m;
    auto fut = QtConcurrent::run([this, item] {
        QImage frame;
        if (item.kind == MediaKind::Image || item.kind == MediaKind::Svg) {
            MediaCache cache;
            frame = cache.stillImage(item.path, item.kind, 384);
        } else if (item.hasVideo) {
            frame = grabFrame(item.path, qMin(1.0, item.duration * 0.1), 384);
        }
        QMetaObject::invokeMethod(
            this,
            [this, frame, id = item.id] {
                m_thumbs[id] = paintIcon(frame, Theme::audioClip(), frame.isNull()
                                                                       ? "♪"
                                                                       : QString());
                refresh();
            },
            Qt::QueuedConnection);
    });
    Q_UNUSED(fut);
}

void MediaBin::refresh() {
    const QString filter = m_search->text().trimmed();
    m_list->clear();
    for (const auto &seq : m_doc->project().sequences) {
        if (seq.id.startsWith(QLatin1String("__"))) continue;  // internal
        if (!filter.isEmpty() && !seq.name.contains(filter, Qt::CaseInsensitive))
            continue;
        auto *it = new QListWidgetItem(seq.name);
        it->setData(Qt::UserRole, "sequence:" + seq.id);
        it->setIcon(paintIcon(QImage(), Theme::nestedClip(), "▦"));
        it->setToolTip(tr("Sequence — %1×%2 @ %3 fps")
                           .arg(seq.width)
                           .arg(seq.height)
                           .arg(seq.fps));
        m_list->addItem(it);
    }
    for (const auto &m : m_doc->project().media) {
        if (!filter.isEmpty() && !m.name.contains(filter, Qt::CaseInsensitive))
            continue;
        auto *it = new QListWidgetItem(m.name);
        it->setData(Qt::UserRole, "media:" + m.id);
        if (m_thumbs.contains(m.id)) {
            it->setIcon(m_thumbs[m.id]);
        } else {
            it->setIcon(paintIcon(QImage(),
                                  m.hasVideo ? Theme::videoClip() : Theme::audioClip(),
                                  m.hasVideo ? "▶" : "♪"));
            if (!m.offline) makeThumbnail(m.id);
        }
        QString tip = m.path;
        if (m.offline) {
            tip = tr("OFFLINE — %1").arg(m.path);
            it->setForeground(QColor(230, 90, 90));
        }
        it->setToolTip(tip);
        m_list->addItem(it);
    }
}

void MediaBin::contextMenu(const QPoint &pos) {
    QListWidgetItem *it = m_list->itemAt(pos);
    QMenu menu(this);
    QAction *import = menu.addAction(tr("Import…"));
    QAction *newSeqFrom = nullptr, *remove = nullptr;
    QString ref = it ? it->data(Qt::UserRole).toString() : QString();
    if (ref.startsWith("media:")) {
        newSeqFrom = menu.addAction(tr("New Sequence from Clip"));
        remove = menu.addAction(tr("Remove"));
    } else if (ref.startsWith("sequence:")) {
        remove = menu.addAction(tr("Open in Timeline"));
    }
    QAction *chosen = menu.exec(m_list->mapToGlobal(pos));
    if (!chosen) return;
    if (chosen == import) {
        importFilesDialog();
    } else if (newSeqFrom && chosen == newSeqFrom) {
        m_doc->sequenceFromMedia(ref.mid(6));
    } else if (remove && chosen == remove) {
        if (ref.startsWith("media:"))
            m_doc->removeMedia(ref.mid(6));
        else
            m_doc->openSequenceTab(ref.mid(9));
    }
}

void MediaBin::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) e->acceptProposedAction();
}

void MediaBin::dropEvent(QDropEvent *e) {
    QStringList paths;
    for (const QUrl &u : e->mimeData()->urls())
        if (u.isLocalFile()) paths << u.toLocalFile();
    if (!paths.isEmpty()) {
        m_doc->beginUndoStep();
        for (const QString &id : m_doc->importMedia(paths)) makeThumbnail(id);
    }
}
