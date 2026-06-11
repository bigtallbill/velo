import json

import os
D = os.path.dirname(os.path.abspath(__file__))
_id = [100]
def cid():
    _id[0] += 1
    return str(_id[0])

def P(base, keys=None):
    o = {"base": base}
    if keys: o["keys"] = [[t, v] for t, v in keys]
    return o

def clip(type_, name, start, dur, media="", **kw):
    c = {
        "id": cid(), "type": type_, "mediaId": media, "name": name,
        "start": start, "duration": dur, "in": kw.get("in_", 0),
        "speed": kw.get("speed", 1.0), "linkId": "0", "enabled": True,
        "posX": kw.get("posX", P(0)), "posY": kw.get("posY", P(0)),
        "scaleX": kw.get("scale", P(1)), "scaleY": kw.get("scale", P(1)),
        "rotation": kw.get("rotation", P(0)),
        "opacity": kw.get("opacity", P(1)), "volume": kw.get("volume", P(1)),
        "uniformScale": True, "effects": kw.get("effects", []),
        "transIn": kw.get("transIn", {"type": 0, "duration": 1}),
        "transOut": kw.get("transOut", {"type": 0, "duration": 1}),
    }
    if "text" in kw: c["text"] = kw["text"]
    return c

def fx(eid, **params):
    return {"effectId": eid, "enabled": True,
            "params": {k: P(v) for k, v in params.items()}}

def track(type_, name, clips, volume=None):
    return {"type": type_, "name": name, "muted": False, "locked": False,
            "volume": volume or P(1), "clips": clips}

def media(mid, fname, name, kind, dur, w, h, fps, hv, ha):
    return {"id": mid, "path": f"{D}/{fname}", "name": name, "kind": kind,
            "duration": dur, "width": w, "height": h, "fps": fps,
            "hasVideo": hv, "hasAudio": ha, "srcIn": 0, "srcOut": -1}

medias = [
    media("m_mand", "mandelbrot.mp4", "Fractal Zoom.mp4", 0, 14, 1280, 720, 30, True, False),
    media("m_grad", "gradients.mp4", "Sunset Flow.mp4", 0, 14, 1280, 720, 30, True, False),
    media("m_life", "life.mp4", "Cellular.mp4", 0, 14, 1280, 720, 30, True, False),
    media("m_music", "music.wav", "Score.wav", 1, 24, 0, 0, 0, False, True),
    media("m_pulse", "pulse.wav", "Pulse Kit.wav", 1, 24, 0, 0, 0, False, True),
    media("m_poster", "poster.png", "Poster.png", 2, 5, 800, 800, 0, True, False),
    media("m_logo", "logo.svg", "Velo Logo.svg", 3, 5, 96, 96, 0, True, False),
]

# ---- nested sequence ------------------------------------------------------
nest = {
    "id": "seq_nest", "name": "Intro Nest", "width": 1280, "height": 720,
    "fps": 30.0,
    "videoTracks": [
        track(0, "V1", [clip(0, "Sunset Flow", 0, 8, "m_grad",
                             effects=[fx("color_correction", brightness=8,
                                         contrast=12, saturation=140,
                                         temperature=15)])]),
        track(0, "V2", [clip(3, "Nested Title", 0.5, 7, text={
            "text": "NESTED COMP", "family": "DejaVu Sans", "pixelSize": 110,
            "bold": True, "italic": False, "color": "#ffffffff",
            "outlineColor": "#ff111133", "outlineWidth": 4})]),
    ],
    "audioTracks": [
        track(1, "A1", [clip(1, "Pulse Kit", 0, 8, "m_pulse")]),
    ],
}

# ---- main sequence ---------------------------------------------------------
v1 = [
    clip(0, "Fractal Zoom", 0, 8, "m_mand", scale=P(1.5)),
    clip(0, "Cellular", 8, 8, "m_life", scale=P(1.5),
         transIn={"type": 1, "duration": 1.4}),     # cross dissolve at cut
    clip(0, "Sunset Flow", 16, 6, "m_grad", in_=2, scale=P(1.5),
         transIn={"type": 2, "duration": 0.8},      # fade in
         transOut={"type": 3, "duration": 1.0}),    # dip to black out
]
pip = clip(0, "Sunset Flow PiP", 2, 8, "m_grad", in_=4,
           scale=P(0.40), posX=P(580), posY=P(-280),
           rotation=P(0, [(0, -7), (8, 7)]),
           opacity=P(1, [(0, 0), (0.8, 1)]),
           effects=[fx("gaussian_blur", radius=14), fx("vignette", amount=72)])
v2 = [
    pip,
    clip(2, "Poster", 12.5, 5.5, "m_poster",
         scale=P(1, [(0, 0.28), (5.5, 0.42)]), posX=P(-560), posY=P(-250),
         rotation=P(-4), opacity=P(0.92)),
]
v3 = [
    clip(4, "Intro Nest", 4, 8, "seq_nest", scale=P(0.34), posX=P(-560),
         posY=P(290), opacity=P(1, [(0, 0), (0.6, 1)])),
    clip(2, "Velo Logo", 13, 6, "m_logo", scale=P(2.6), posX=P(560),
         posY=P(280)),
]
v4 = [
    clip(3, "Title", 0.5, 6.5,
         posX=P(-260), posY=P(0, [(0, -170), (1.4, -305)]),
         opacity=P(1, [(0, 0), (1.0, 1), (5.6, 1), (6.4, 0)]),
         text={"text": "VELO", "family": "DejaVu Sans", "pixelSize": 230,
               "bold": True, "italic": False, "color": "#ffffffff",
               "outlineColor": "#ff2a6fd6", "outlineWidth": 7}),
    clip(3, "Subtitle", 7, 5.5, posY=P(-320),
         opacity=P(1, [(0, 0), (0.6, 1)]),
         text={"text": "Non-linear  ·  Fast  ·  Yours",
               "family": "DejaVu Sans", "pixelSize": 64, "bold": False,
               "italic": True, "color": "#ffe8ecf4",
               "outlineColor": "#aa000000", "outlineWidth": 2}),
]
a1 = [clip(1, "Score", 0, 22, "m_music",
           volume=P(1, [(0, 0.0), (1.5, 1.0), (9.5, 1.0), (11.5, 0.4),
                        (16, 0.95), (21.5, 0.0)]))]
a2 = [clip(1, "Pulse Kit", 4, 12, "m_pulse", in_=2,
           transIn={"type": 2, "duration": 1.2},
           effects=[fx("gain", db=-4)])]

main = {
    "id": "seq_main", "name": "Showcase", "width": 1920, "height": 1080,
    "fps": 30.0,
    "videoTracks": [track(0, "V1", v1), track(0, "V2", v2),
                    track(0, "V3", v3), track(0, "V4", v4)],
    "audioTracks": [track(1, "A1", a1),
                    track(1, "A2", a2, volume=P(1, [(4, 0.7), (10, 1.0)]))],
}

proj = {
    "app": "velo", "version": 1,
    "media": medias,
    "sequences": [main, nest],
    "openTabs": ["seq_main", "seq_nest"],
    "activeSequence": "seq_main",
    "nextClipId": "9999",
}
open(f"{D}/showcase.velo", "w").write(json.dumps(proj, indent=1))
print("pip clip id:", pip["id"])
