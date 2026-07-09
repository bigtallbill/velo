# Quick start

This walkthrough takes you from an empty project to an exported video in
about five minutes.

## 1. Import some media

Press ++ctrl+i++ (or click **Import…** in the Project panel) and pick a few
video files. They appear in the bin with thumbnails.

Even quicker: drag files — or a whole folder — from your file manager
straight into the Project panel. Folders are scanned recursively and their
structure is recreated as [bin folders](../interface/project-panel.md).

## 2. Create a sequence

The easiest way: **right-click a clip in the bin → New Sequence from
Clip**. That creates a sequence matching the clip's resolution and frame
rate and drops the clip at its start.

Or press ++ctrl+n++ to create one manually — pick a delivery preset
(YouTube 1080p, TikTok/Reels, Cinema 4K DCI, …) or enter custom
dimensions. See [Sequences](../editing/sequences.md) for the full preset
list.

You can also simply **drag a clip from the bin into the empty timeline** —
Velo builds a matching sequence for you.

## 3. Preview and trim your source

Double-click a clip in the bin (or drop it onto the monitor) to open it in
the **Media Preview** monitor. Scrub through it, then press ++i++ and
++o++ to set in and out points. When you drag the picture from the Media
Preview into the timeline, only the in→out range is inserted.

## 4. Edit in the timeline

- **Move** clips by dragging; they snap magnetically to edits and the
  playhead (++n++ toggles snapping, hold ++alt++ to bypass it).
- **Trim** by dragging a clip's edge — the cursor turns into a bracket.
- **Split** with the razor tool (++c++) or press ++s++ to split the
  selection at the playhead (++shift+s++ splits every track). Switch back
  to the selection tool with ++v++.
- **Delete** with ++del++, or ++alt+del++ to ripple delete (later clips
  shift left to close the gap).
- Video and its audio are **linked** — they move, trim and delete
  together. Right-click → *Unlink* when you need them apart.

## 5. Add polish

- Drag a **transition** (Cross Dissolve, Fade, Dip to Black) from the
  Effects panel onto a clip edge.
- Drag an **effect** (Gaussian Blur, Color Grade, …) onto a clip, then
  tune its parameters in **Effect Controls**.
- Press ++t++ to add a **title** at the playhead and style it in Effect
  Controls.
- Select a clip and reposition it right in the **Sequence monitor**; drag
  the corner handles to scale.

## 6. Export

Press ++ctrl+m++. Pick a codec (H.264 is the safe default), check the
resolution and quality, and click **Export**. A progress bar tracks the
render; the result plays anywhere.

## 7. Save your project

Press ++ctrl+s++. Everything — sequences, edits, keyframes, effects —
lands in a single portable `.velo` file. Undo history is snapshot-based,
so experiment freely: ++ctrl+z++ / ++ctrl+shift+z++ walk you back and
forward.

!!! tip "Try the showcase project"
    The repository ships a script (`dist/demo/make_demo.sh`) that generates
    a demo project showing nested sequences, picture-in-picture, keyframed
    effects, titles and transitions — a good way to explore a finished
    edit.
