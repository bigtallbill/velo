# Applying effects

The **Effects** tab (sharing the top-left panel with the Project bin)
lists every effect and transition, grouped by category: Blur, Color,
Transform, Stylize, Audio and Transitions.

## Adding an effect to a clip

- **Drag** the effect from the panel onto a clip in the timeline, or
- select the clip first and **double-click** the effect in the panel.

The effect appears as a section in
[Effect Controls](../interface/effect-controls.md) with its parameters.

## Managing applied effects

In Effect Controls, each effect block provides:

- **Enable checkbox** — bypass the effect without losing its settings;
  great for before/after comparisons.
- **Remove** — delete the effect from the clip.
- **Order** — effects render **top to bottom**. Order matters: a vignette
  before a blur looks different from a blur before a vignette.

## Keyframing parameters

Every effect parameter carries the standard stopwatch/diamond controls —
animate blur radius, grade intensity, crop edges, anything. See
[Keyframes & animation](../editing/keyframes.md).

## Effects on anything

Effects apply to every video-kind clip: footage, images, SVG, titles and
**nested sequences** — grade an entire section by nesting it first, then
applying one Color Grade to the nest.

## Audio effects

Audio effects (currently **Gain**) apply the same way, to audio clips.
They appear in the same Effect Controls list.

## See also

- [Effect reference](reference.md) — every built-in effect with its
  parameters, ranges and defaults.
- [Transitions](transitions.md) — Cross Dissolve, Fade and Dip to Black
  work differently from clip effects: they attach to clip *edges*.
