#pragma once
#include "core/Document.h"
#include <QSet>
#include <QTreeWidget>
#include <QWidget>

class QLineEdit;

// Project panel: imported media + sequences in bin folders, with thumbnails.
// Items drag onto the timeline with the custom mime type
// "application/x-velo-item" whose payload is "media:<id>" or "sequence:<id>"
// ("folder:<path>" only re-arranges within the bin).
class MediaBin : public QWidget {
    Q_OBJECT
public:
    explicit MediaBin(Document *doc, QWidget *parent = nullptr);
    void importFilesDialog(const QString &bin = QString());
    void importFolderDialog(const QString &bin = QString());

signals:
    void previewRequested(const QString &mediaId);  // double-clicked media

private:
    friend class MediaTreeWidget;
    void refresh();
    void makeThumbnail(const QString &mediaId);
    void contextMenu(const QPoint &pos);
    void removeRef(const QString &ref);
    void deleteSelectedItems();
    void newFolder(const QString &parent);
    // Bin folder an item lives in (folders: their own path; null: root).
    QString binForItem(QTreeWidgetItem *it) const;

    Document *m_doc;
    class MediaTreeWidget *m_tree;
    QLineEdit *m_search;
    QHash<QString, QIcon> m_thumbs;
    QSet<QString> m_collapsed;  // folder paths the user collapsed
    bool m_refreshing = false;

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

// QTreeWidget that emits our custom drag mime data, accepts drops of files
// and directories (import) and of bin items (move into folder), and
// handles Del.
class MediaTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit MediaTreeWidget(MediaBin *bin) : m_bin(bin) {}

protected:
    QMimeData *mimeData(const QList<QTreeWidgetItem *> &items) const override;
    bool event(QEvent *e) override;          // claim Del via ShortcutOverride
    void keyPressEvent(QKeyEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;

private:
    MediaBin *m_bin;
};
