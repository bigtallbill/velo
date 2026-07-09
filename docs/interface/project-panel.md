# Project panel

The Project panel (top left) is Velo's media bin: every imported file and
every sequence lives here, organized into folders, with thumbnails and
search.

## Importing media

There are several ways to bring media in:

- **Import button** — click **Import…** for files, or open its dropdown
  for **Import Files…** / **Import Folder…**.
- **Keyboard** — ++ctrl+i++ imports files, ++ctrl+shift+i++ imports a
  whole folder.
- **Drag and drop** — drop files *or directories* from your file manager
  anywhere onto the panel. Dropping onto a specific folder imports into
  that folder.
- **Context menu** — right-click anywhere in the bin for *Import…*,
  *Import Folder…* and *New Folder*.

### Folder import

When you import a directory, Velo scans it **recursively**, picks up every
supported media file, and recreates the directory structure as folders in
the bin. Unsupported files are skipped silently; hidden files are ignored.
Files that are already in the project are not imported twice.

### Supported formats

| Type | Extensions |
|---|---|
| Video | `mp4`, `mov`, `mkv`, `webm`, `avi`, `m4v`, `mts` |
| Audio | `mp3`, `wav`, `flac`, `aac`, `ogg`, `opus`, `m4a` |
| Images | `png`, `jpg`, `jpeg`, `webp`, `bmp`, `tif`, `tiff`, `gif` |
| Vector | `svg`, `svgz` |

When importing individual files you can also switch the file dialog to
*All files* — anything FFmpeg can decode will generally work.

## Folders

Folders keep large projects manageable:

- **Create** — right-click → *New Folder* (inside the folder you clicked,
  or at the root). The name is immediately editable.
- **Rename** — click a selected folder (or press ++f2++) and type; items
  inside follow along.
- **Move things** — drag media into a folder to move it there; drag a
  folder into another folder to nest it. Drop onto empty space to move
  items back to the root.
- **Remove** — ++del++ or right-click → *Remove Folder* deletes the folder
  **including all media inside it** (their timeline clips are removed
  too). Empty folders are kept in the project and survive saving.

## Working with items

- **Thumbnails** — video and image items show a frame; audio items show a
  note icon.
- **Search** — the search box filters the whole tree as you type; matching
  folders and items stay visible.
- **Rename** — click a selected item or press ++f2++; renaming media only
  changes its display name, not the file on disk.
- **Preview** — double-click media (or drag it onto the monitor) to open
  it in the [Media Preview](media-preview.md).
- **To the timeline** — drag items into the [timeline](timeline.md).
  Dragging into an *empty* project creates a matching sequence
  automatically.
- **New Sequence from Clip** — right-click a media item to create a
  sequence with exactly its resolution and frame rate.
- **Multi-select** — ++ctrl++-click or ++shift++-click to select several
  items; ++del++ or right-click → *Remove Selected* removes them all.
- **Sequences** appear in the bin alongside media — double-click one to
  open it as a timeline tab.

## Offline media

If a project references files that have moved or been deleted, those items
turn **red** and are flagged *OFFLINE*. Right-click → **Locate File…** to
point them at the file's new location — every clip that uses the item
picks up the new file, keeping your edit intact. The same menu offers
**Replace File…** for online media, which swaps the underlying file while
keeping the item's name, in/out points and all timeline usage.
