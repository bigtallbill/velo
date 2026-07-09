# Transitions

Transitions blend a clip's edge — into the neighboring clip, or from/to
black. Velo ships three:

| Transition | Effect |
|---|---|
| **Cross Dissolve** | Blends the outgoing clip into the incoming one |
| **Fade (in/out)** | Fades the clip edge from/to transparency — lower tracks (or black) show through |
| **Dip to Black** | Fades fully to black, then back up |

## Applying

- **Drag** a transition from the Effects panel onto a **clip edge** — the
  end of one clip or the start of the next, or
- right-click a clip → apply from the context menu.

Dropping **near a cut** between two adjacent clips spans the transition
across both — that's the classic cross dissolve. Dropping on an edge with
nothing adjacent creates a fade in or out at that edge.

## Adjusting

- In the timeline, a transition renders as a **wedge** on the clip edge —
  **drag the wedge** to change its length.
- In [Effect Controls](../interface/effect-controls.md), the
  *Transitions* section shows the incoming and outgoing transition with a
  numeric duration (default 1 s).

## Removing

Right-click the clip and remove the transition from the context menu, or
set its type back to none in Effect Controls.

## Notes

- Cross dissolves need enough **source material past the cut** on both
  clips — the outgoing clip keeps playing under the incoming one.
- Transitions work on every video-kind clip, including titles and nested
  sequences. A Fade on a title is the fastest fade-in/out.
- For audio crossfades, use [volume-line keyframes](../editing/audio.md#the-volume-line)
  on the overlapping clips.
