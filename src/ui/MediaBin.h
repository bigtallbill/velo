#pragma once
#include "core/Document.h"
#include <QListWidget>
#include <QWidget>

class QLineEdit;

// Project panel: imported media + sequences, with thumbnails. Items drag
// onto the timeline with the custom mime type "application/x-velo-item"
// whose payload is "media:<id>" or "sequence:<id>".
class MediaBin : public QWidget {
    Q_OBJECT
public:
    explicit MediaBin(Document *doc, QWidget *parent = nullptr);
    void importFilesDialog();

signals:
    void previewRequested(const QString &mediaId);  // double-clicked media

private:
    void refresh();
    void makeThumbnail(const QString &mediaId);
    void contextMenu(const QPoint &pos);

    Document *m_doc;
    QListWidget *m_list;
    QLineEdit *m_search;
    QHash<QString, QIcon> m_thumbs;

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;
};

// QListWidget that emits our custom drag mime data.
class MediaListWidget : public QListWidget {
    Q_OBJECT
public:
    using QListWidget::QListWidget;

protected:
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;
};
