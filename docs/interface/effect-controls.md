# Effect Controls

The Effect Controls panel (top right) edits everything about the selected
clip: transform, opacity, volume, speed, text style and the parameters of
every applied effect. Select a clip in the timeline — or a track header
for track-level controls — and the panel populates.

## Sections

- **Transform** — position X/Y (offset from the sequence center, in
  pixels), scale X/Y, rotation in degrees, and a *uniform scale* toggle.
- **Opacity** — 0–100 % clip opacity.
- **Volume** — clip gain (audio clips and A/V pairs).
- **Time** — clip **speed** and the **Preserve pitch** toggle: at
  non-100 % speeds, audio is either resampled (chipmunk/slow-motion
  pitch) or time-stretched to keep its original pitch. See
  [Working with clips](../editing/clips.md#speed).
- **Text** — font family, size, bold/italic, fill color, outline color and
  width (text clips only). See [Titles](../editing/titles.md).
- **Effects** — one collapsible block per applied effect, in render
  order, each with its parameters, an enable checkbox and a remove
  button. The parameter UI is generated from the effect's description, so
  every effect — including ones you add to the source — gets the same
  treatment.
- **Transitions** — duration controls for the clip's incoming and
  outgoing transition.

## Editing values

- **Scrub** — drag any numeric label left or right to change its value;
  hold ++shift++ for 10× coarser steps.
- **Type** — click the value and enter a number.
- Values applied by dragging in the
  [Sequence monitor](sequence-monitor.md) (position, scale) show up here
  live.

## Keyframing

Every parameter with a **stopwatch** icon can animate:

1. Click the stopwatch to enable keyframing — a keyframe is created at
   the playhead with the current value.
2. Move the playhead and change the value — a new keyframe is added
   automatically.
3. Use the **diamond** button to add or remove a keyframe exactly at the
   playhead.

Interpolation between keyframes is **smooth-step** — eases out of the
first keyframe and into the next. Keyframe times are stored relative to
the clip, so keyframes travel with the clip when you move it. For the
full picture see [Keyframes & animation](../editing/keyframes.md).
