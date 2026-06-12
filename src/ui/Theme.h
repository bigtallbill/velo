#pragma once
#include <QApplication>
#include <QIcon>

namespace Theme {
void apply(QApplication &app);

// Bundled monochrome SVG icon (resources/icons/<name>.svg), cached.
// All UI glyphs come from here so they render identically on every OS.
QIcon icon(const QString &name);

// shared palette constants for custom-painted widgets
inline QColor panel() { return QColor(0x23, 0x25, 0x29); }
inline QColor panelDark() { return QColor(0x1b, 0x1d, 0x20); }
inline QColor border() { return QColor(0x3a, 0x3d, 0x42); }
inline QColor accent() { return QColor(0x4f, 0x9c, 0xf5); }
inline QColor textDim() { return QColor(0x9a, 0x9e, 0xa6); }
inline QColor videoClip() { return QColor(0x3f, 0x6e, 0x9e); }
inline QColor audioClip() { return QColor(0x3e, 0x8e, 0x5c); }
inline QColor textClip() { return QColor(0x9e, 0x6e, 0xc4); }
inline QColor nestedClip() { return QColor(0xb5, 0x7c, 0x35); }
}  // namespace Theme
