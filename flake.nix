{
  description = "the kl, meet";
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
      dir = pkgs.gcc13Stdenv.mkDerivation {
        name = "ascip";
        meta.description = "ascip is a clean cpp ascii parser, it doesn't requires any changes in using code, has no dependecies. single header file.";
        src = ./.;
      };
    in rec {
      devShell = dir;
      packages.kl = dir;
    });
}
