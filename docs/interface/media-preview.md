# Media Preview monitor

The Media Preview is the left tab of the monitor area. It plays source
media *before* it's in the timeline, so you can pick exactly the range you
want.

## Loading media

- **Double-click** a media item in the Project panel, or
- **drag** it from the bin onto the monitor.

The dropdown above the picture keeps a list of everything you've loaded —
switch between sources without going back to the bin, or click the **✕**
to unload one. Still images display without a transport.

## Playback

The transport works like the sequence monitor: ++space++ plays and pauses,
++left++ / ++right++ step one frame (++shift++ for five), ++home++ /
++end++ jump to the start and end. Dragging the playhead scrubs — with
audible audio if [audio scrubbing](../editing/audio.md#audio-scrubbing) is
on.

## In and out points

Below the picture is a mini-timeline with two draggable brackets:

- press ++i++ to set the **in point** at the playhead,
- press ++o++ to set the **out point**,
- drag the brackets to fine-tune the range,
- ++ctrl+shift+x++ clears both.

In/out points are stored per media item and saved with the project.

## Inserting into the timeline

Drag **the picture itself** into the timeline: only the in→out range is
inserted, already trimmed. If no points are set, the whole clip is
inserted. New clips created by dragging the item from the *bin* honor the
same range.

!!! tip
    Set rough in/out points in the Media Preview, insert, then fine-tune
    with [edge trimming](timeline.md#trimming) in the timeline — that's
    usually the fastest workflow for interview-style footage.
