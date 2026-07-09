{
  description = "Velo — a fast, friendly non-linear video editor (Qt 6 + FFmpeg)";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      forAllSystems = f: nixpkgs.lib.genAttrs systems
        (system: f nixpkgs.legacyPackages.${system});
    in
    {
      packages = forAllSystems (pkgs: {
        # End-user documentation (MkDocs Material): nix build .#docs
        docs = pkgs.runCommand "velo-docs"
          {
            nativeBuildInputs = [
              (pkgs.python3.withPackages (ps: [ ps.mkdocs ps.mkdocs-material ]))
            ];
          } ''
          cd ${self}
          mkdocs build --strict --site-dir $out
        '';

        default = pkgs.stdenv.mkDerivation {
          pname = "velo";
          version = "0.0.1"; # keep in sync with project(... VERSION) in CMakeLists.txt

          src = self;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
            qt6.wrapQtAppsHook
          ];

          buildInputs = with pkgs; [
            qt6.qtbase
            qt6.qtmultimedia
            qt6.qtsvg
            ffmpeg
          ];

          # Export renders through a piped `ffmpeg` process, so the CLI
          # must be reachable from the wrapped binary.
          qtWrapperArgs = [
            "--prefix PATH : ${pkgs.lib.makeBinPath [ pkgs.ffmpeg ]}"
          ];

          meta = {
            description = "Fast, friendly non-linear video editor built with C++20, Qt 6 and FFmpeg";
            homepage = "https://github.com/notune/velo";
            license = pkgs.lib.licenses.gpl3Only;
            mainProgram = "velo";
            platforms = pkgs.lib.platforms.linux;
          };
        };
      });

      devShells = forAllSystems (pkgs: {
        default = pkgs.mkShell {
          inputsFrom = [ self.packages.${pkgs.stdenv.hostPlatform.system}.default ];
          packages = [
            pkgs.ffmpeg # ffmpeg CLI for export
            # docs: mkdocs serve / mkdocs build
            (pkgs.python3.withPackages (ps: [ ps.mkdocs ps.mkdocs-material ]))
          ];

          # The freshly built ./build/velo is unwrapped, so point Qt at the
          # platform/imageformat/multimedia plugins from the shell instead.
          shellHook = ''
            export QT_PLUGIN_PATH=${pkgs.lib.makeSearchPath pkgs.qt6.qtbase.qtPluginPrefix
              (with pkgs.qt6; [ qtbase qtsvg qtmultimedia ])}
          '';
        };
      });
    };
}
