# Titles

Text in Velo is just another clip type — titles sit on video tracks,
composite over everything below, and animate like any other clip.

## Adding text

Press ++t++ (or *Sequence → Add Text at Playhead*). A text clip appears at
the playhead on a video track, ready to edit.

## Styling

Select the text clip; the **Text** section of
[Effect Controls](../interface/effect-controls.md) offers:

- the text content itself,
- **font family**, **size** (pixels), **bold** and *italic*,
- **fill color**,
- **outline color** and **outline width** — set a width above 0 for
  outlined text that stays readable over busy footage.

## Text clips are normal clips

Everything that works on video clips works on titles:

- **Move / trim** them in the timeline to control when they appear.
- **Position, scale, rotate** them in the monitor or Effect Controls —
  and [keyframe](keyframes.md) all of it for animated lower-thirds.
- **Opacity keyframes** make classic fade-in/fade-out titles, or drop a
  [Fade transition](../effects/transitions.md) on the clip's edges for
  the same result in one drag.
- **Effects** apply too — a subtle Gaussian Blur behind a drop shadow
  look, or Flip for mirrored text.

!!! example "Simple lower-third"
    1. ++t++ to add text; type the name, set a size around 60 px, bold.
    2. Drag it to the lower-left in the monitor.
    3. Keyframe *Position X* to slide it in from off-screen over 12
       frames.
    4. Add a **Fade** transition on the out edge so it leaves cleanly.

## Reusing styled titles

Copy/paste a text clip to reuse its styling — the paste carries the full
style and any keyframes. For a title you use constantly, keep a styled
text clip in a dedicated ["templates" sequence](sequences.md) and copy
from there.
