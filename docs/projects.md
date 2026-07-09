# Projects & undo

## The .velo file

A Velo project is a single **JSON file** with the `.velo` extension. It
stores everything: media references (with their bin folders and in/out
points), sequences, clips, keyframes, effects, transitions, text styles
and open tabs. Because it's plain JSON, projects diff cleanly in version
control and can be inspected or repaired in a text editor.

- **Save** — ++ctrl+s++; **Save As** — ++ctrl+shift+s++.
- **Open** — ++ctrl+o++; you can also pass a project on the command line:
  `velo myproject.velo`.
- **Open Recent** — *File → Open Recent* lists your last ten projects
  (missing files are hidden); *Clear List* empties it.
- **New project** — ++ctrl+alt+n++.

The window title shows the project name with a `*` for unsaved changes,
and Velo asks before discarding them (closing, opening another project,
or creating a new one).

## Media references

Projects store **paths** to your media, not the media itself. Keep your
footage where it was, or expect to relink:

- Items whose files are missing appear **red / OFFLINE** in the bin.
- Right-click → **Locate File…** points the item at the new location;
  every clip that uses it picks up the change — the edit is preserved.

Moving a whole project between machines works well when media lives in
one folder next to the project file (relative layout preserved).

## Undo and redo

Undo is **snapshot-based**: every editing operation stores a full project
snapshot, so undo is always exact — no operation is too complex to
reverse.

- ++ctrl+z++ — undo (up to 100 steps),
- ++ctrl+shift+z++ — redo.

Undo covers edits to sequences, clips, keyframes, effects, media
(import/remove/rename, bin folders) — the whole project state. The redo
stack clears when you make a new edit after undoing.

## What isn't in the project file

A few things are per-user settings, not project data:

- [custom keyboard shortcuts](shortcuts.md#customizing),
- audio scrubbing preference and the audio output device,
- window layout.

These live in your platform's settings store (on Linux:
`~/.config/velo/velo.conf`).
