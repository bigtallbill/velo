# Installation

Prebuilt, fully self-contained bundles for Linux, Windows and macOS are on
the [releases page](https://github.com/notune/velo/releases/latest). Qt, the
FFmpeg libraries **and the `ffmpeg` export binary** are bundled at the exact
versions Velo is developed against, so nothing else needs to be installed.

## Linux

Two bundle formats are published:

- **AppImage** — `Velo-<version>-linux-x86_64.AppImage`. Make it executable
  and run it:

  ```bash
  chmod +x Velo-*-linux-x86_64.AppImage
  ./Velo-*-linux-x86_64.AppImage
  ```

  On systems without FUSE, add `--appimage-extract-and-run`.

- **Portable archive** — `Velo-<version>-linux-x86_64-portable.tar.xz`.
  Extract anywhere and run `./AppRun`.

!!! note "glibc requirement"
    The Linux bundles need glibc ≥ 2.39 — that means Ubuntu 24.04 or newer,
    or any 2024+ rolling distribution. On older systems, build from source
    instead.

## Windows

Download `Velo-<version>-windows-x86_64.zip`, extract it anywhere and run
`velo.exe`. No installer is required.

## macOS (Apple Silicon)

Download `Velo-<version>-macos-arm64.dmg` and drag Velo to Applications.
The build is unsigned, so on first launch right-click the app and choose
*Open* — or remove the quarantine flag:

```bash
xattr -d com.apple.quarantine /Applications/Velo.app
```

## Building from source

Velo is a CMake project needing Qt 6 (Base, Multimedia, Svg), FFmpeg
libraries, and a C++20 compiler. On Arch-based distributions:

```bash
pacman -S qt6-base qt6-multimedia qt6-svg ffmpeg cmake ninja gcc pkgconf
cmake -B build -G Ninja
ninja -C build
./build/velo
```

Install system-wide (binary, desktop entry and icon):

```bash
sudo cmake --install build --prefix /usr
```

### NixOS / Nix

With Nix and flakes enabled, no packages need to be installed manually:

```bash
nix run github:notune/velo        # run directly
nix build                         # build ./result/bin/velo in a checkout
nix develop                       # dev shell for iterating with cmake/ninja
```

The `nix build` output is fully wrapped — Qt plugins resolve on their own
and the `ffmpeg` binary needed for export is on the app's PATH.

### Verifying a build

A headless engine smoke test (decode → composite → mix → save/load →
export) is built in:

```bash
QT_QPA_PLATFORM=offscreen ./build/velo --selftest
```

It should print `selftest: OK`.

## The ffmpeg export binary

Exporting renders through a separate `ffmpeg` process. Velo looks for it in
this order:

1. the `VELO_FFMPEG` environment variable (full path to a binary),
2. an `ffmpeg` binary next to the Velo executable (this is what the
   official bundles ship),
3. `ffmpeg` on your PATH.

If none is found, export fails with *"Could not start ffmpeg"* — see the
[FAQ](../faq.md) for fixes.
