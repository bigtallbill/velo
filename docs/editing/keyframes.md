# Keyframes & animation

Nearly every numeric value in Velo can animate: position, scale, rotation,
opacity, clip volume, track volume — and **every parameter of every
effect**. The workflow is identical everywhere.

## Creating keyframes

In [Effect Controls](../interface/effect-controls.md), each animatable
parameter has two buttons:

- **Stopwatch** — turns animation on for that parameter. The moment you
  enable it, a keyframe with the current value is created at the
  playhead. Turning the stopwatch *off* removes the animation and returns
  to a single static value.
- **Diamond** — adds a keyframe at the playhead (or removes the one
  that's already there).

With the stopwatch on, **changing a value automatically keys it** at the
playhead — position the playhead, set the value, move on. This includes
changes made by dragging the clip or its handles in the
[Sequence monitor](../interface/sequence-monitor.md).

## Interpolation

Values between keyframes follow a **smooth-step** curve: motion eases out
of each keyframe and into the next, so animation feels natural without
any curve editing. Before the first keyframe the first value holds; after
the last keyframe the last value holds.

## Timing

Keyframe times are **clip-local** — they stick to the clip as you move it
around the timeline, and they stretch with [speed changes](clips.md#speed).
The exception is **track volume**, whose keyframes live on the sequence
clock — see [Audio](audio.md#track-volume).

## Volume keyframes on the clip

Audio clips offer a faster path than the panel: the **volume line** drawn
across the waveform. ++ctrl++-click the line to add a keyframe, drag the
dots to shape the level, double-click a dot to remove it. See
[Audio](audio.md#the-volume-line).

## Common recipes

!!! example "Picture-in-picture fly-in"
    1. Place the PiP clip on a track above the main footage.
    2. Enable the stopwatch on *Position* and *Scale*.
    3. At the entrance frame, set the start position (off-screen) and a
       small scale.
    4. Half a second later, set the final position and size. Done — the
       smooth-step easing handles the rest.

!!! example "Animated blur reveal"
    1. Apply **Gaussian Blur** to the clip.
    2. Enable the stopwatch on *Radius*.
    3. Key radius 60 at the cut, radius 0 a second in.

!!! example "Audio duck under a voice-over"
    ++ctrl++-click the music clip's volume line four times around the
    speech: full → low → low → full. Drag the middle dots down.
