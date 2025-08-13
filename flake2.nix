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
        buildInputs = [
          pkgs.clang
          pkgs.raylib
          pkgs.pkg-config
        ];

        shellHook = ''
          echo "Entered N-body simulation dev shell (Raylib + Clang ready)"
          echo "Compiling..."
          clang -Wall -Wextra -Werror main.c nbody.c nbody_config.c solar_system_conf.c $(pkg-config --cflags --libs raylib) -lm -o nbody.elf
          echo "Build complete. Run it with: ./nbody.elf"
        '';
      };
    };
}
