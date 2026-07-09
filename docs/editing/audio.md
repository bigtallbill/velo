# Audio

Velo mixes audio sample-accurately across all tracks, with per-clip and
per-track gain, keyframes on both, and pitch-preserving speed changes.

## Waveforms

Audio clips draw their waveform in the timeline, so edit points are easy
to spot. Waveform peaks are computed in the background after import.

## The volume line

Each audio clip carries a horizontal **volume line**:

- **Drag the line** up or down to set the clip's overall level.
- **++ctrl++-click** the line to add a **keyframe** (a dot).
- **Drag dots** to shape the level over time — fades, ducks, swells.
- **Double-click a dot** to remove it.

The same value is editable numerically (and keyframable) as *Volume* in
[Effect Controls](../interface/effect-controls.md).

## Track volume

Audio track headers include a track-level volume. Unlike clip keyframes,
**track-volume keyframes run on sequence time** — they stay put while you
rearrange clips underneath. Use it to ride the level of a whole music bed
or dialog bus.

## Gain effect

For extra headroom, apply the **Gain** effect (−48 to +24 dB) from the
Effects panel — it stacks with clip and track volume, and its dB value is
keyframable like any other effect parameter.

## Speed and pitch

Changing a clip's speed affects its audio in one of two ways, chosen by
*Effect Controls → Time → Preserve pitch*:

- **Resample** (default) — pitch shifts with speed, like tape.
- **Preserve pitch** — a time-stretcher keeps the original pitch. Use for
  speech at moderate speed changes.

## Audio scrubbing

With *Playback → Audio Scrubbing* enabled (default **on**), dragging the
playhead plays the audio under it — invaluable for finding a word or a
beat. The timeline toolbar has a matching toggle; the two stay in sync.

## Output device

*Playback → Audio Output* lists every output device Qt can see. Pick one
or leave *System default*; the change applies the next time playback
starts.

## Audio-only export

The [export dialog](../export.md) has an **Audio only** checkbox that
renders just the mix — handy for podcast edits done in the timeline.
