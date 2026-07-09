# FAQ & troubleshooting

## Installation

??? question "The AppImage won't start"
    Most often this is a missing FUSE setup. Run it with
    `./Velo-*.AppImage --appimage-extract-and-run`, or use the portable
    `.tar.xz` bundle instead (extract and run `./AppRun`).

??? question "The Linux bundle complains about glibc"
    The official bundles need glibc ≥ 2.39 (Ubuntu 24.04+, any 2024+
    rolling release). On older distributions,
    [build from source](getting-started/installation.md#building-from-source)
    — Velo itself has no such floor.

??? question "macOS says the app is damaged or can't be opened"
    The build is unsigned. Right-click the app → *Open* the first time,
    or clear the quarantine flag:
    `xattr -d com.apple.quarantine /Applications/Velo.app`.

## Media

??? question "My media shows red / OFFLINE"
    The file moved or the project came from another machine. Right-click
    the item → **Locate File…** and point it at the file's new location.
    Every clip that uses the item follows — your edit is intact.

??? question "A file won't import"
    Check it plays in another FFmpeg-based player. When using the import
    dialog, switch the filter to *All files* — the extension list is a
    convenience, and anything FFmpeg can decode is fair game. Files with
    only cover art (e.g. some MP3s) import as audio, not video.

??? question "Folder import missed some files"
    Directory scans only pick up
    [supported extensions](interface/project-panel.md#supported-formats)
    and skip hidden files. Import stragglers individually via the file
    dialog with the *All files* filter.

## Editing

??? question "Clips jump around while dragging"
    That's magnetic snapping. Press ++n++ to toggle it, or hold ++alt++
    during the drag to bypass it once.

??? question "Why won't my clip trim any longer?"
    Trimming can't extend past the source media. To hold a frame longer,
    use a still (image) or slow the clip down
    ([speed](editing/clips.md#speed)).

??? question "Deleting one half of a clip deletes the other too"
    Video and audio from the same file are linked. Right-click →
    **Unlink** to separate them.

??? question "The preview is choppy on my 4K sequence"
    Drop the monitor's **Quality** to 1/2 or 1/4 — preview-only, export
    is always full quality. Nesting effect-heavy sections also helps.

## Export

??? question "Export fails with 'Could not start ffmpeg'"
    Velo can't find the `ffmpeg` binary. The official bundles ship one;
    if you built from source, install ffmpeg on your PATH or set
    `VELO_FFMPEG=/path/to/ffmpeg`. See
    [how Velo finds ffmpeg](export.md#how-velo-finds-ffmpeg).

??? question "NVENC doesn't appear in the codec list"
    The list shows what your `ffmpeg` reports. NVENC needs an NVIDIA GPU,
    a driver with the encoder enabled, and an ffmpeg built with NVENC
    support. Otherwise use x264 — it's excellent, just slower.

??? question "Which settings for YouTube?"
    H.264, CRF 18–20, resolution and fps matching your sequence, audio
    192 kbps. For 4K, H.265 or AV1 saves significant upload size.

## Projects

??? question "Can I edit a .velo file by hand?"
    Yes — it's indented JSON. Velo also tolerates hand-added media
    entries and bin folders. Keep a backup, and prefer the app for
    anything structural.

??? question "Where are my custom shortcuts stored?"
    Per user, outside the project — on Linux in
    `~/.config/velo/velo.conf`. Delete the `shortcuts/` entries there to
    reset everything to defaults.

## Getting help

Bug reports and feature requests are welcome on the
[GitHub issue tracker](https://github.com/notune/velo/issues).
