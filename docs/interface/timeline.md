# Timeline

The timeline is the bottom half of the window: video tracks stack above
the audio tracks, sequences open as tabs, and the toolbar carries the
tools, snapping toggle and zoom controls.

## Tracks

- **Unlimited tracks** — add more with *Sequence → Add Video Track*
  (++ctrl+shift+v++) or *Add Audio Track* (++ctrl+shift+a++). Video track
  V1 is the **bottom** of the render stack; higher tracks composite on
  top.
- **Track headers** offer per-track controls: **mute** (audio) / **hide**
  (video), **lock** (prevents edits), **rename**, and a track **volume**
  with sequence-time keyframes for audio tracks — see
  [Audio](../editing/audio.md).
- Every sequence starts with three video and three audio tracks.

## Selecting

- Click a clip to select it; ++ctrl++-click adds to the selection.
- **Rubber-band** — drag on empty timeline space to select everything the
  rectangle touches.
- ++ctrl+a++ selects all clips; clicking a track header selects the track.

## Moving and snapping

Drag clips along a track or between tracks of the same type. **Magnetic
snapping** pulls clip edges to other edits, the playhead and markers:

- ++n++ toggles snapping on and off,
- holding ++alt++ bypasses it for one drag.

## Trimming

Hover a clip's edge until the cursor becomes a bracket, then drag to trim
the head or tail. Trimming respects the source length — you can't extend a
clip past its media. For linked A/V pairs both halves trim together.

## Splitting

- **Razor tool** — press ++c++, then click a clip to cut it at that point.
  Press ++v++ to return to the selection tool.
- **Split at playhead** — ++s++ splits the selected clips; ++shift+s++
  splits **all** tracks at the playhead.

Split halves of a linked A/V pair stay linked to their own halves.

## Deleting and gaps

- ++del++ removes the selected clips and leaves a gap.
- ++alt+del++ **ripple deletes** — later clips on the affected tracks
  shift left to close the gap.
- Right-click a gap → *Close Gap* shifts later clips left to fill it.

## Linked audio and video

Media with both video and audio comes in as a **linked pair**: moving,
trimming, splitting and deleting affect both. To separate them,
right-click → **Unlink**; to re-join a video and an audio clip,
select both and right-click → **Link**.

## Copy and paste

++ctrl+c++ copies the selection; ++ctrl+v++ pastes at the playhead on the
original tracks, times normalized so the earliest copied clip lands at the
playhead. Paste works across sequences.

## Nesting ("Chain")

Select clips and press ++alt+c++ to **collapse them into a single
nested-sequence clip**. The nest behaves like any other clip — trim it,
move it, add effects and transitions — and **double-clicking it opens the
nested sequence in its own tab** for editing. Edits inside propagate to
every place the nest is used. See also
[Sequences](../editing/sequences.md#nested-sequences).

## Transitions in the timeline

Applied transitions render as a **wedge** at the clip edge. Drag the wedge
to adjust the duration, or edit it numerically in Effect Controls. See
[Transitions](../effects/transitions.md).

## Zoom and navigation

| Control | Action |
|---|---|
| ++equal++ / ++minus++ | Zoom in / out |
| ++backslash++ | Zoom to fit the whole sequence |
| ++ctrl++ + mouse wheel | Zoom at the cursor position |
| Mouse wheel | Scroll |

## Sequence tabs

Each open sequence is a tab above the timeline. Double-click sequences in
the bin (or nested clips in the timeline) to open more; close tabs you
don't need — the sequence stays in the project.
