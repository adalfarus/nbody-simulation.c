{
  description = "N-body simulation dev environment with Raylib";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.${system}.default = pkgs.mkShell {
        # pkg-config belongs in nativeBuildInputs; compilers/libs in buildInputs
        nativeBuildInputs = [
          pkgs.pkg-config
        ];
        buildInputs = [
          pkgs.clang
          pkgs.raylib
          pkgs.raygui
        ];

        # -std=c99 -O3 -g
        CFLAGS = "-Wall -Wextra";  #  -Werror RayGUI does not compile otherwise
        LDFLAGS = "-lm";

        #shellHook = ''
        #  echo "Entered N-body simulation dev shell (Raylib + Clang ready)"
        #  echo "Compiling..."
        #  clang -Wall -Wextra -Werror main.c nbody.c nbody_config.c solar_system_conf.c $(pkg-config --cflags --libs raylib) -lm -o nbody.elf
        #  echo "Build complete. Run it with: ./nbody.elf"
        #'';
        shellHook = ''
          echo "Entered N-body simulation dev shell (Raylib + Clang ready)"
          echo "Type:  build    -> compile"
          echo "       run      -> ./nbody.elf"
          echo "       clean    -> remove artifacts"

          build () {
            echo "Compiling..."
            clang $CFLAGS \
              main.c nbody.c nbody_config.c \
              $(pkg-config --cflags --libs raylib) $LDFLAGS \
              -o nbody.elf
            echo "Build complete."
          }

          run () {
            if [ -x ./nbody.elf ]; then
              ./nbody.elf
            else
              echo "Executable not found. Run 'build' first."
            fi
          }

          clean () {
            rm -f nbody.elf
          }
        '';
      };
    };
}
