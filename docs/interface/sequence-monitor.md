# Sequence monitor

The Sequence tab of the monitor area shows the rendered timeline at the
playhead — everything composited: all tracks, transforms, effects and
transitions.

## Playback

| Control | Action |
|---|---|
| ++space++ | Play / pause |
| ++left++ / ++right++ | Step one frame |
| ++shift+left++ / ++shift+right++ | Step five frames |
| ++home++ / ++end++ | Go to start / end |

The timecode readout shows the playhead position as
`hours:minutes:seconds:frames` at the sequence frame rate. Dragging the
playhead in the timeline scrubs the picture (and the audio, if audio
scrubbing is enabled).

## Direct manipulation

Select a clip in the timeline and you can adjust it right in the picture:

- **Move** — drag the clip's image to change its position.
- **Scale** — drag the corner handles. Scaling is uniform by default; hold
  ++shift++ to distort (scale X and Y independently).
- **Rotation and opacity** live in
  [Effect Controls](effect-controls.md), along with numeric entry for
  position and scale.

Changes made in the monitor are keyframed automatically when the
corresponding parameter's stopwatch is enabled — see
[Keyframes & animation](../editing/keyframes.md).

## Preview quality

Rendering full-resolution previews of a 4K timeline is expensive. The
**Quality** dropdown in the monitor's corner switches the preview
resolution between **Full**, **1/2**, **1/4** and **1/8** — export quality
is never affected. Drop it down when scrubbing feels sluggish on heavy
sequences.

!!! note
    Preview renders coalesce: while you scrub, Velo only renders the most
    recent frame you asked for, so the monitor never lags behind by a
    queue of stale frames.
