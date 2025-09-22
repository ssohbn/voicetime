{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      lib = pkgs.lib;
      stdenv = pkgs.stdenv;
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = builtins.attrValues {
          inherit (pkgs) clang-tools gcc flac;
        };
        LD_LIBRARY_PATH = ''${lib.makeLibraryPath [ pkgs.flac ]}:$LD_LIBRARY_PATH'';

      };
      packages.${system}.voicetime = stdenv.mkDerivation (finalAttrs: {
        pname = "voicetimer";
        version = "0.1";

        src = ./.;

        buildInputs = [
          pkgs.flac
        ];

	installPhase = ''
	mkdir -p $out/bin
	ls
	mv voicetime $out/bin
	'';
      });
    };
}
