{
  description = "N-body simulation dev environment with Raylib";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs   = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      # pkg-config belongs in nativeBuildInputs; compilers/libs in buildInputs
      nativeBuildInputs = [
        pkgs.pkg-config
      ];

      buildInputs = [
        pkgs.clang
        pkgs.raylib
      ];

      # Optional: handy flags
      CFLAGS = "-std=c11 -O2 -g -Wall -Wextra -Werror";
      LDFLAGS = "-lm";

      # Provide simple build/run helpers instead of building on shell entry
      shellHook = ''
        echo "Entered N-body simulation dev shell (Raylib + Clang ready)"
        echo "Type:  build    -> compile"
        echo "       run      -> ./raygui.elf"
        echo "       clean    -> remove artifacts"

        build () {
          echo "Compiling..."
          clang $CFLAGS \
            raygui_example.c \
            $(pkg-config --cflags --libs raylib) $LDFLAGS \
            -o raygui.elf
          echo "Build complete."
        }

        run () {
          if [ -x ./raygui.elf ]; then
            ./raygui.elf
          else
            echo "Executable not found. Run 'build' first."
          fi
        }

        clean () {
          rm -f raygui.elf
        }
      '';
    };
  };
}
