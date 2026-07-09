# Exporting

Press ++ctrl+m++ (or *File → Export…*) to render the active sequence to a
file. Export runs through a piped `ffmpeg` process with live progress and
cancel — the app stays responsive.

## The export dialog

| Field | Meaning |
|---|---|
| **Output file** | Destination path; the extension follows the codec (`.mp4`, `.webm` for VP9, `.mov` for ProRes) but you can override it |
| **Video codec** | Every encoder your ffmpeg supports, from the list below |
| **Resolution / fps** | Prefilled from the sequence; export at any size and rate — the render is scaled to fit |
| **Quality** | CRF 0–51 (**lower is better**); default 20 |
| **Audio bitrate** | 64–512 kbps, default 192 |
| **Audio only** | Skip video and render just the audio mix |

## Codecs

| Codec | Notes |
|---|---|
| **H.264 (x264)** | The safe default — plays everywhere |
| **H.265 / HEVC (x265)** | Half the size of H.264 at similar quality; slower to encode |
| **H.264 / HEVC (NVIDIA NVENC)** | Hardware encoding on NVIDIA GPUs — much faster, slightly larger files. Only listed when your ffmpeg and driver support it |
| **VP9 (WebM)** | Open format for the web |
| **AV1 (SVT)** | Best compression, newest format |
| **Apple ProRes** | Intermediate/mastering format (ProRes HQ); large files, ideal for further editing. Quality slider does not apply |

The codec list is probed from the `ffmpeg` binary at startup — if
something is missing, your ffmpeg build doesn't include that encoder (see
the [FAQ](faq.md)).

## Quality: CRF in one paragraph

CRF (constant rate factor) targets constant *quality* instead of constant
bitrate: the encoder spends bits where the picture needs them. Lower
values mean better quality and larger files. Sensible ranges: **17–20**
for high-quality masters, **20–23** for general delivery, **26+** when
size matters more than fidelity. For NVENC the value maps to the
equivalent `-cq` rate control.

## Audio

The mix is rendered sample-accurately first (you'll see a *Mixing audio*
stage), then encoded to **AAC** — or **Opus** when exporting `.webm` or
`.ogg` — at the chosen bitrate.

**Audio only** renders no video at all: name the output `.m4a` (AAC) or
`.ogg`/`.webm` (Opus) as you prefer.

## During the export

The progress bar tracks the encode; the label shows the current stage.
**Close** cancels a running export safely. Settings are locked while an
export runs.

## How Velo finds ffmpeg

1. `VELO_FFMPEG` environment variable (full path),
2. an `ffmpeg` next to the Velo executable — the official bundles ship
   one at the exact tested version,
3. `ffmpeg` on PATH.
