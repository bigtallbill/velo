# Sequences

A sequence is one timeline: a resolution, a frame rate, and a stack of
video and audio tracks. Projects can hold any number of sequences, and
sequences can contain other sequences (nesting).

## Creating a sequence

Three ways:

- **From a clip (recommended)** — right-click media in the bin → **New
  Sequence from Clip**. The sequence matches the clip's resolution and
  frame rate exactly, and the clip is placed at its start. No settings to
  get wrong.
- **Manually** — *File → New Sequence…* (++ctrl+n++). Pick a preset or
  enter custom dimensions (16–16384 px, 1–240 fps).
- **By dropping** — drag media into the empty timeline of a fresh project
  and Velo builds a matching sequence automatically.

## Delivery presets

| Preset | Resolution | fps |
|---|---|---|
| YouTube 1080p | 1920 × 1080 | 30 |
| YouTube 1080p 60 | 1920 × 1080 | 60 |
| YouTube 4K (UHD) | 3840 × 2160 | 30 |
| Cinema 4K DCI 24 | 4096 × 2160 | 24 |
| Film 1080p 24 | 1920 × 1080 | 24 |
| PAL 1080p 25 | 1920 × 1080 | 25 |
| TikTok / Reels / Shorts | 1080 × 1920 (vertical) | 30 |
| Instagram Square | 1080 × 1080 | 30 |
| 720p | 1280 × 720 | 30 |
| Custom | anything | anything |

Editing the width, height or fps of a preset automatically switches the
dialog to *Custom*.

## Working with multiple sequences

- Sequences appear in the **Project panel** next to your media —
  double-click to open one as a timeline tab, or right-click → *Open in
  Timeline*.
- Rename a sequence by clicking its selected bin entry (or ++f2++).
- Deleting a sequence (++del++ in the bin) removes it from the project —
  and removes any nested clips that referenced it.

## Nested sequences

Select clips in the timeline and press ++alt+c++ ("Chain") to collapse
them into a **nested-sequence clip**:

- The nest is a normal clip: move it, trim it, speed-change it, stack
  effects and transitions on it, keyframe its transform.
- **Double-click** the nested clip to open the inner sequence in a tab;
  edits inside show up everywhere the nest is used.
- Nests can contain other nests.
- You can also drag any sequence from the bin into another sequence's
  timeline to nest it there.

!!! tip "When to nest"
    Nesting is the way to treat a finished section as one unit — apply a
    single color grade across it, transition into it as a whole, or reuse
    an animated lower-third in several places.
