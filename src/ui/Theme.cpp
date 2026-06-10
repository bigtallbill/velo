#include "ui/Theme.h"
#include <QPalette>
#include <QStyleFactory>

void Theme::apply(QApplication &app) {
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    const QColor window(0x2a, 0x2c, 0x30), base(0x1f, 0x21, 0x24);
    const QColor text(0xe8, 0xe9, 0xeb), dim(0x9a, 0x9e, 0xa6);
    p.setColor(QPalette::Window, window);
    p.setColor(QPalette::WindowText, text);
    p.setColor(QPalette::Base, base);
    p.setColor(QPalette::AlternateBase, window);
    p.setColor(QPalette::Text, text);
    p.setColor(QPalette::Button, window);
    p.setColor(QPalette::ButtonText, text);
    p.setColor(QPalette::BrightText, Qt::white);
    p.setColor(QPalette::Highlight, accent());
    p.setColor(QPalette::HighlightedText, Qt::black);
    p.setColor(QPalette::ToolTipBase, base);
    p.setColor(QPalette::ToolTipText, text);
    p.setColor(QPalette::PlaceholderText, dim);
    p.setColor(QPalette::Link, accent());
    p.setColor(QPalette::Disabled, QPalette::Text, dim.darker(130));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, dim.darker(130));
    p.setColor(QPalette::Disabled, QPalette::WindowText, dim.darker(130));
    app.setPalette(p);
    app.setStyleSheet(R"(
        QToolTip { background: #1f2124; color: #e8e9eb; border: 1px solid #3a3d42; }
        QSplitter::handle { background: #1b1d20; }
        QDockWidget::title { background: #232529; padding: 4px 8px; }
        QTabBar::tab { background: #232529; color: #9a9ea6; padding: 4px 14px;
                       border: 1px solid #1b1d20; border-bottom: none; }
        QTabBar::tab:selected { background: #2f3236; color: #e8e9eb; }
        QMenu { background: #26282c; border: 1px solid #3a3d42; }
        QMenu::item:selected { background: #4f9cf5; color: black; }
        QSlider::groove:horizontal { height: 4px; background: #1b1d20; border-radius: 2px; }
        QSlider::handle:horizontal { width: 12px; margin: -5px 0; border-radius: 6px;
                                     background: #c8cacd; }
        QSlider::sub-page:horizontal { background: #4f9cf5; border-radius: 2px; }
        QScrollBar { background: #1f2124; }
        QScrollBar::handle { background: #3f4248; border-radius: 4px; min-width: 24px; min-height: 24px; }
        QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; }
    )");
}
