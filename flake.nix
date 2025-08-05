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
          clang -Wall -Wextra -Werror n_body.c n_body_wrapper.c $(pkg-config --cflags --libs raylib) -lm -o nbody
          echo "Build complete. Run it with: ./nbody"
        '';
      };
    };
}
