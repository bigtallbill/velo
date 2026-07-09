# Working with clips

Everything on the timeline is a clip: video, audio, images, SVG, text and
nested sequences. This page covers the operations that apply to all of
them; timeline mechanics (moving, trimming, splitting) are described in
[Timeline](../interface/timeline.md).

## Clip types

| Type | Source | Notes |
|---|---|---|
| Video | video files | Usually paired with a linked audio clip |
| Audio | audio files or the audio half of an A/V pair | Waveform display, volume line |
| Image | png/jpg/webp/… | Default 5 s duration, trim to any length |
| SVG | svg/svgz | Rendered sharply at any scale |
| Text | created with ++t++ | Styled in Effect Controls |
| Nested | other sequences | Double-click to open the inner sequence |

## Transform and opacity

Select a clip and use [Effect Controls](../interface/effect-controls.md)
— or drag directly in the
[Sequence monitor](../interface/sequence-monitor.md):

- **Position** — X/Y offset in pixels from the sequence center.
- **Scale** — uniform by default; disable *uniform scale* (or hold
  ++shift++ while dragging a corner handle) to stretch.
- **Rotation** — degrees, keyframable for spins.
- **Opacity** — fade or blend clips stacked on higher tracks.

All of these animate — see [Keyframes](keyframes.md).

## Speed

*Effect Controls → Time* sets the clip's playback **speed**. The clip's
timeline duration changes accordingly (200 % speed halves it).

For clips with audio, the **Preserve pitch** toggle picks the algorithm:

- **Off (resample)** — classic varispeed: faster playback raises the
  pitch, slower lowers it.
- **On (time-stretch)** — audio keeps its original pitch while playing
  faster or slower. Best for speech.

## Enabling and disabling

A disabled clip stays in the timeline but doesn't render or sound —
useful for A/B comparisons. Toggle it from the clip's context menu.

## Copy and paste

++ctrl+c++ / ++ctrl+v++ copy whole clips **including** their effects,
keyframes, transitions and text styling. Paste lands at the playhead, on
the same tracks, in any open sequence.

## Renaming

Clips take their name from their media item (or their text content for
titles). Rename media in the bin; nested clips display their sequence's
name.

## Removing media from under clips

Deleting a media item in the bin removes every timeline clip that used
it. Deleting only some clips of a media item leaves the rest — and the
bin item — untouched.
