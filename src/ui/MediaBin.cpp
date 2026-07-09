#include "ui/MediaBin.h"
#include "media/MediaCache.h"
#include "ui/Theme.h"
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QToolButton>
#include <QTreeWidgetItemIterator>
#include <QVBoxLayout>
#include <QtConcurrent>

static const char *kMime = "application/x-velo-item";

static QString refOf(const QTreeWidgetItem *it) {
    return it ? it->data(0, Qt::UserRole).toString() : QString();
}

QMimeData *MediaTreeWidget::mimeData(const QList<QTreeWidgetItem *> &items) const {
    auto *mime = new QMimeData;
    if (!items.isEmpty())
        mime->setData(kMime, refOf(items.first()).toUtf8());
    return mime;
}

bool MediaTreeWidget::event(QEvent *e) {
    if (e->type() == QEvent::ShortcutOverride) {
        auto *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Delete && !selectedItems().isEmpty()) {
            e->accept();  // keep the global "delete clips" action out of it
            return true;
        }
    }
    return QTreeWidget::event(e);
}

void MediaTreeWidget::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Delete && !selectedItems().isEmpty()) {
        m_bin->deleteSelectedItems();
        return;
    }
    QTreeWidget::keyPressEvent(e);
}

void MediaTreeWidget::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls() || e->mimeData()->hasFormat(kMime))
        e->acceptProposedAction();
}

void MediaTreeWidget::dragMoveEvent(QDragMoveEvent *e) {
    QTreeWidget::dragMoveEvent(e);  // drop indicator + hover auto-expand
    if (e->mimeData()->hasUrls() || e->mimeData()->hasFormat(kMime))
        e->acceptProposedAction();
}

void MediaTreeWidget::dropEvent(QDropEvent *e) {
    const QString bin = m_bin->binForItem(itemAt(e->position().toPoint()));
    if (e->mimeData()->hasUrls()) {
        QStringList paths;
        for (const QUrl &u : e->mimeData()->urls())
            if (u.isLocalFile()) paths << u.toLocalFile();
        if (!paths.isEmpty()) {
            m_bin->m_doc->beginUndoStep();
            for (const QString &id : m_bin->m_doc->importMedia(paths, bin))
                m_bin->makeThumbnail(id);
        }
        e->acceptProposedAction();
        return;
    }
    if (e->mimeData()->hasFormat(kMime)) {
        // move the selected bin entries into the target folder
        QStringList mediaIds, folderPaths;
        for (QTreeWidgetItem *it : selectedItems()) {
            const QString ref = refOf(it);
            if (ref.startsWith("media:")) mediaIds << ref.mid(6);
            else if (ref.startsWith("folder:")) folderPaths << ref.mid(7);
        }
        if (!mediaIds.isEmpty()) m_bin->m_doc->moveMediaToBin(mediaIds, bin);
        for (const QString &p : std::as_const(folderPaths))
            m_bin->m_doc->moveBinFolder(p, bin);
        e->acceptProposedAction();
    }
}

MediaBin::MediaBin(Document *doc, QWidget *parent) : QWidget(parent), m_doc(doc) {
    setAcceptDrops(true);  // drops on the panel margins import to the root
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(4, 4, 4, 4);
    lay->setSpacing(4);

    auto *bar = new QHBoxLayout;
    auto *importBtn = new QToolButton;
    importBtn->setText(tr("Import…"));
    importBtn->setToolTip(tr("Import media files (Ctrl+I)"));
    importBtn->setPopupMode(QToolButton::MenuButtonPopup);
    auto *importMenu = new QMenu(importBtn);
    importMenu->addAction(tr("Import Files…"), this, [this] { importFilesDialog(); });
    importMenu->addAction(tr("Import Folder…"), this, [this] { importFolderDialog(); });
    importBtn->setMenu(importMenu);
    connect(importBtn, &QToolButton::clicked, this, [this] { importFilesDialog(); });
    m_search = new QLineEdit;
    m_search->setPlaceholderText(tr("Search…"));
    m_search->setClearButtonEnabled(true);
    connect(m_search, &QLineEdit::textChanged, this, &MediaBin::refresh);
    bar->addWidget(importBtn);
    bar->addWidget(m_search, 1);
    lay->addLayout(bar);

    m_tree = new MediaTreeWidget(this);
    m_tree->setHeaderHidden(true);
    m_tree->setIconSize(QSize(64, 36));
    m_tree->setIndentation(14);
    m_tree->setDragEnabled(true);
    m_tree->viewport()->setAcceptDrops(true);
    m_tree->setDropIndicatorShown(true);
    m_tree->setAutoExpandDelay(500);
    m_tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tree->setTextElideMode(Qt::ElideMiddle);
    // click an already-selected item (or F2) to rename it in place
    m_tree->setEditTriggers(QAbstractItemView::SelectedClicked |
                            QAbstractItemView::EditKeyPressed);
    lay->addWidget(m_tree, 1);

    connect(m_tree, &QTreeWidget::itemChanged, this, [this](QTreeWidgetItem *it) {
        if (m_refreshing) return;
        const QString ref = refOf(it);
        const QString name = it->text(0).trimmed();
        if (name.isEmpty()) {
            refresh();
            return;
        }
        if (ref.startsWith("sequence:"))
            m_doc->renameSequence(ref.mid(9), name);
        else if (ref.startsWith("media:"))
            m_doc->renameMedia(ref.mid(6), name);
        else if (ref.startsWith("folder:"))
            m_doc->renameBinFolder(ref.mid(7), name);
    });
    connect(m_tree, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem *it) {
        if (!m_refreshing) m_collapsed.remove(binForItem(it));
    });
    connect(m_tree, &QTreeWidget::itemCollapsed, this, [this](QTreeWidgetItem *it) {
        if (!m_refreshing) m_collapsed.insert(binForItem(it));
    });

    connect(m_tree, &QTreeWidget::customContextMenuRequested, this,
            &MediaBin::contextMenu);
    connect(m_tree, &QTreeWidget::itemDoubleClicked, this,
            [this](QTreeWidgetItem *it) {
                const QString ref = refOf(it);
                if (ref.startsWith("sequence:"))
                    m_doc->openSequenceTab(ref.mid(9));
                else if (ref.startsWith("media:"))
                    emit previewRequested(ref.mid(6));
            });
    connect(m_doc, &Document::mediaChanged, this, &MediaBin::refresh);
    connect(m_doc, &Document::sequenceListChanged, this, &MediaBin::refresh);
    connect(m_doc, &Document::projectLoaded, this, [this] {
        m_thumbs.clear();
        m_collapsed.clear();
        refresh();
    });
    refresh();
}

void MediaBin::importFilesDialog(const QString &bin) {
    const QStringList files = QFileDialog::getOpenFileNames(
        this, tr("Import Media"), QDir::homePath(), mediaFileDialogFilter());
    if (!files.isEmpty()) {
        m_doc->beginUndoStep();
        const QStringList ids = m_doc->importMedia(files, bin);
        for (const QString &id : ids) makeThumbnail(id);
    }
}

void MediaBin::importFolderDialog(const QString &bin) {
    const QString dir = QFileDialog::getExistingDirectory(
        this, tr("Import Media Folder"), QDir::homePath());
    if (!dir.isEmpty()) {
        m_doc->beginUndoStep();
        const QStringList ids = m_doc->importMedia({dir}, bin);
        for (const QString &id : ids) makeThumbnail(id);
    }
}

static QIcon paintIcon(const QImage &frame, const QColor &fallback,
                       const QString &iconName) {
    QImage img(192, 108, QImage::Format_ARGB32_Premultiplied);
    img.fill(fallback);
    QPainter p(&img);
    if (!frame.isNull()) {
        QImage s = frame.scaled(img.size(), Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);
        p.fillRect(img.rect(), QColor(20, 20, 22));
        p.drawImage((img.width() - s.width()) / 2, (img.height() - s.height()) / 2, s);
    } else if (!iconName.isEmpty()) {
        const int s = 52;
        p.setOpacity(0.75);
        Theme::icon(iconName).paint(
            &p, QRect((img.width() - s) / 2, (img.height() - s) / 2, s, s));
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
                                                                       ? "note"
                                                                       : QString());
                refresh();
            },
            Qt::QueuedConnection);
    });
    Q_UNUSED(fut);
}

void MediaBin::refresh() {
    m_refreshing = true;
    const QString filter = m_search->text().trimmed();
    m_tree->clear();

    // Folder items, parents created on demand. With a search filter only
    // folders that match (or contain a match) appear.
    QHash<QString, QTreeWidgetItem *> folders;
    auto folderItem = [&](const QString &path) -> QTreeWidgetItem * {
        QTreeWidgetItem *it = nullptr;
        QString acc;
        for (const QString &part : path.split('/', Qt::SkipEmptyParts)) {
            acc = acc.isEmpty() ? part : acc + '/' + part;
            if (QTreeWidgetItem *have = folders.value(acc)) {
                it = have;
                continue;
            }
            auto *child = it ? new QTreeWidgetItem(it)
                             : new QTreeWidgetItem(m_tree);
            child->setText(0, part);
            child->setFlags(child->flags() | Qt::ItemIsEditable);
            child->setData(0, Qt::UserRole, "folder:" + acc);
            child->setIcon(0, Theme::icon("folder"));
            folders[acc] = child;
            it = child;
        }
        return it;
    };

    for (const auto &seq : m_doc->project().sequences) {
        if (seq.id.startsWith(QLatin1String("__"))) continue;  // internal
        if (!filter.isEmpty() && !seq.name.contains(filter, Qt::CaseInsensitive))
            continue;
        auto *it = new QTreeWidgetItem(m_tree);
        it->setText(0, seq.name);
        it->setFlags((it->flags() | Qt::ItemIsEditable) & ~Qt::ItemIsDropEnabled);
        it->setData(0, Qt::UserRole, "sequence:" + seq.id);
        it->setIcon(0, paintIcon(QImage(), Theme::nestedClip(), "nested"));
        it->setToolTip(0, tr("Sequence — %1×%2 @ %3 fps")
                              .arg(seq.width)
                              .arg(seq.height)
                              .arg(seq.fps));
    }

    QStringList bins = m_doc->project().bins;
    bins.sort();
    for (const QString &b : std::as_const(bins))
        if (filter.isEmpty() ||
            b.mid(b.lastIndexOf('/') + 1).contains(filter, Qt::CaseInsensitive))
            folderItem(b);

    for (const auto &m : m_doc->project().media) {
        if (!filter.isEmpty() && !m.name.contains(filter, Qt::CaseInsensitive))
            continue;
        auto *it = m.bin.isEmpty() ? new QTreeWidgetItem(m_tree)
                                   : new QTreeWidgetItem(folderItem(m.bin));
        it->setText(0, m.name);
        it->setFlags((it->flags() | Qt::ItemIsEditable) & ~Qt::ItemIsDropEnabled);
        it->setData(0, Qt::UserRole, "media:" + m.id);
        if (m_thumbs.contains(m.id)) {
            it->setIcon(0, m_thumbs[m.id]);
        } else {
            it->setIcon(0, paintIcon(QImage(),
                                     m.hasVideo ? Theme::videoClip()
                                                : Theme::audioClip(),
                                     m.hasVideo ? "play" : "note"));
            if (!m.offline) makeThumbnail(m.id);
        }
        QString tip = m.path;
        if (m.offline) {
            tip = tr("OFFLINE — %1").arg(m.path);
            it->setForeground(0, QColor(230, 90, 90));
        }
        it->setToolTip(0, tip);
    }

    for (auto f = folders.constBegin(); f != folders.constEnd(); ++f)
        f.value()->setExpanded(!filter.isEmpty() || !m_collapsed.contains(f.key()));
    m_refreshing = false;
}

QString MediaBin::binForItem(QTreeWidgetItem *it) const {
    for (; it; it = it->parent()) {
        const QString ref = refOf(it);
        if (ref.startsWith("folder:")) return ref.mid(7);
    }
    return {};
}

void MediaBin::newFolder(const QString &parent) {
    QString path;
    for (int n = 1;; ++n) {
        const QString name =
            n == 1 ? tr("New Folder") : tr("New Folder %1").arg(n);
        path = parent.isEmpty() ? name : parent + '/' + name;
        if (!m_doc->project().bins.contains(path)) break;
    }
    m_doc->createBinFolder(path);  // emits mediaChanged -> refresh
    for (QTreeWidgetItemIterator iter(m_tree); *iter; ++iter)
        if (refOf(*iter) == "folder:" + path) {
            m_tree->setCurrentItem(*iter);
            m_tree->editItem(*iter);
            break;
        }
}

void MediaBin::contextMenu(const QPoint &pos) {
    QTreeWidgetItem *it = m_tree->itemAt(pos);
    const QString ref = refOf(it);
    const QString targetBin = binForItem(it);
    // right-clicking inside a multi-selection: Remove acts on all of it
    const bool multi =
        it && it->isSelected() && m_tree->selectedItems().size() > 1;
    QMenu menu(this);
    QAction *import = menu.addAction(tr("Import…"));
    QAction *importFolder = menu.addAction(tr("Import Folder…"));
    QAction *newFolder = menu.addAction(tr("New Folder"));
    QAction *newSeqFrom = nullptr, *open = nullptr, *rename = nullptr,
            *remove = nullptr, *relink = nullptr;
    if (ref.startsWith("media:")) {
        menu.addSeparator();
        const MediaItem *m = m_doc->project().mediaByIdConst(ref.mid(6));
        newSeqFrom = menu.addAction(tr("New Sequence from Clip"));
        relink = menu.addAction(m && m->offline ? tr("Locate File…")
                                                : tr("Replace File…"));
        rename = menu.addAction(tr("Rename"));
        remove = menu.addAction(multi ? tr("Remove Selected\tDel")
                                      : tr("Remove\tDel"));
    } else if (ref.startsWith("sequence:")) {
        menu.addSeparator();
        open = menu.addAction(tr("Open in Timeline"));
        rename = menu.addAction(tr("Rename"));
        remove = menu.addAction(multi ? tr("Remove Selected\tDel")
                                      : tr("Delete Sequence\tDel"));
    } else if (ref.startsWith("folder:")) {
        menu.addSeparator();
        rename = menu.addAction(tr("Rename"));
        remove = menu.addAction(multi ? tr("Remove Selected\tDel")
                                      : tr("Remove Folder\tDel"));
    }
    QAction *chosen = menu.exec(m_tree->mapToGlobal(pos));
    if (!chosen) return;
    if (chosen == import) {
        importFilesDialog(targetBin);
    } else if (chosen == importFolder) {
        importFolderDialog(targetBin);
    } else if (chosen == newFolder) {
        this->newFolder(targetBin);
    } else if (newSeqFrom && chosen == newSeqFrom) {
        m_doc->sequenceFromMedia(ref.mid(6));
    } else if (relink && chosen == relink) {
        const MediaItem *m = m_doc->project().mediaByIdConst(ref.mid(6));
        const QString start =
            m && !m->path.isEmpty() ? QFileInfo(m->path).path() : QDir::homePath();
        const QString file = QFileDialog::getOpenFileName(
            this, tr("Locate Media File"), start, mediaFileDialogFilter());
        if (!file.isEmpty()) {
            if (m_doc->relocateMedia(ref.mid(6), file))
                m_thumbs.remove(ref.mid(6));  // re-thumbnail the new file
            refresh();
        }
    } else if (open && chosen == open) {
        m_doc->openSequenceTab(ref.mid(9));
    } else if (rename && chosen == rename) {
        m_tree->editItem(it);
    } else if (remove && chosen == remove) {
        if (multi) deleteSelectedItems();
        else removeRef(ref);
    }
}

void MediaBin::removeRef(const QString &ref) {
    if (ref.startsWith("media:"))
        m_doc->removeMedia(ref.mid(6));
    else if (ref.startsWith("sequence:"))
        m_doc->removeSequence(ref.mid(9));
    else if (ref.startsWith("folder:"))
        m_doc->removeBinFolder(ref.mid(7));
}

void MediaBin::deleteSelectedItems() {
    QStringList refs;
    for (QTreeWidgetItem *it : m_tree->selectedItems())
        refs << refOf(it);
    for (const QString &ref : std::as_const(refs)) removeRef(ref);
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
