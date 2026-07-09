# Effect reference

Every built-in effect, with parameter ranges and defaults. All parameters
are keyframable.

## Blur

### Gaussian Blur

True gaussian blur, scaled correctly at every preview resolution.

| Parameter | Range | Default |
|---|---|---|
| Radius | 0 – 200 | 10 |

## Color

### Color Correction

Quick fixes: exposure-style brightness, contrast, saturation and
white-balance temperature in one compact effect.

| Parameter | Range | Default |
|---|---|---|
| Brightness | −100 – 100 | 0 |
| Contrast | −100 – 100 | 0 |
| Saturation | 0 – 200 | 100 |
| Temperature | −100 – 100 | 0 |

### Color Grade

The full grading toolkit — tonal controls in the style of a photo editor
plus lift/gamma/gain for shadow/midtone/highlight balance.

| Parameter | Range | Default |
|---|---|---|
| Exposure (EV) | −4 – 4 | 0 |
| Contrast | −100 – 100 | 0 |
| Highlights | −100 – 100 | 0 |
| Shadows | −100 – 100 | 0 |
| Whites | −100 – 100 | 0 |
| Blacks | −100 – 100 | 0 |
| Temperature | −100 – 100 | 0 |
| Tint | −100 – 100 | 0 |
| Hue | −180 – 180 | 0 |
| Saturation | 0 – 200 | 100 |
| Vibrance | −100 – 100 | 0 |
| Lift | −100 – 100 | 0 |
| Gamma | −100 – 100 | 0 |
| Gain | −100 – 100 | 0 |

!!! tip
    *Vibrance* boosts muted colors while sparing already-saturated ones
    and skin tones — usually the safer choice over *Saturation*.

### Sharpen

Unsharp-mask style sharpening.

| Parameter | Range | Default |
|---|---|---|
| Amount | 0 – 300 | 50 |

### Black & White

Desaturation with a mix control — keyframe *Mix* for color-wash-in
reveals.

| Parameter | Range | Default |
|---|---|---|
| Mix | 0 – 100 | 100 |

## Transform

### Crop

Trims a percentage from each edge of the clip's picture.

| Parameter | Range | Default |
|---|---|---|
| Left % | 0 – 100 | 0 |
| Right % | 0 – 100 | 0 |
| Top % | 0 – 100 | 0 |
| Bottom % | 0 – 100 | 0 |

## Stylize

### Vignette

Darkens the frame's corners.

| Parameter | Range | Default |
|---|---|---|
| Amount | 0 – 100 | 40 |

### Flip

Mirrors the picture.

| Parameter | Range | Default |
|---|---|---|
| Horizontal | off / on | on |
| Vertical | off / on | off |

## Audio

### Gain

Adds gain to an audio clip, stacking with clip and track volume.

| Parameter | Range | Default |
|---|---|---|
| Gain (dB) | −48 – 24 | 0 |

## Adding your own

Velo's effects are self-contained descriptions in the source
(`src/effects/Effects.cpp`) — register one and the browser, Effect
Controls UI, keyframing, serialization and rendering pick it up
automatically. See the
[README](https://github.com/notune/velo#adding-a-new-effect) for the
recipe.
