#!/bin/sh
# Generates the showcase demo media (ffmpeg) and the showcase.velo project
# next to this script, then prints how to open it.
set -e
cd "$(dirname "$0")"
ffmpeg -y -loglevel error -f lavfi -i "mandelbrot=size=1280x720:rate=30:end_scale=0.00002" -t 14 -c:v libx264 -pix_fmt yuv420p -preset fast mandelbrot.mp4
ffmpeg -y -loglevel error -f lavfi -i "gradients=size=1280x720:rate=30:speed=0.05:nb_colors=5:c0=0xff5e62:c1=0xff9966:c2=0x6a82fb:c3=0xfc5c7d:c4=0x45b7af" -t 14 -c:v libx264 -pix_fmt yuv420p -preset fast gradients.mp4
ffmpeg -y -loglevel error -f lavfi -i "life=size=1280x720:rate=30:ratio=0.08:mold=12:death_color=#1a1040:life_color=#7fdcff:seed=1234" -t 14 -c:v libx264 -pix_fmt yuv420p -preset fast life.mp4
ffmpeg -y -loglevel error -f lavfi -i "aevalsrc='0.45*sin(2*PI*110*t)*(0.55+0.45*sin(2*PI*0.5*t)) + 0.3*sin(2*PI*220*t)*(0.5+0.5*sin(2*PI*0.23*t+1)) + 0.22*sin(2*PI*330*t)*gt(mod(t,1),0.5)':s=48000" -t 24 music.wav
ffmpeg -y -loglevel error -f lavfi -i "aevalsrc='0.8*sin(2*PI*70*t)*exp(-12*mod(t,0.5)) + 0.25*sin(2*PI*880*t)*exp(-40*mod(t+0.25,0.5))':s=48000" -t 24 pulse.wav
ffmpeg -y -loglevel error -f lavfi -i "gradients=size=800x800:nb_colors=4:c0=0xfcb045:c1=0xfd1d1d:c2=0x833ab4:c3=0x42d4f4" -frames:v 1 poster.png
cp ../velo.svg logo.svg 2>/dev/null || cp "$(dirname "$0")/../velo.svg" logo.svg
python3 make_project.py
echo "Demo ready: open showcase.velo in Velo"
