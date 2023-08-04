{
  description = "cpp parser inspired with boost.spirit, without dependencies and poison";
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
      der = pkgs.gcc13Stdenv.mkDerivation {
        name = "ascip";
        nativeBuildInputs = [pkgs.clang_15];
        installPhase = "mkdir -p \"$out/include\" && cp ascip.hpp -t \"$out/include\"";
        buildPhase = "g++ -std=c++23 -fwhole-program -march=native ./test.cpp -o ascip_test && ./ascip_test";
        meta.description = "ascip is a clean cpp ascii parser, it doesn't requires any changes in using code, has no dependecies. single header file.";
        src = ./.;
      };
    in rec {
      devShell = der;
      packages.default = der;
      packages.ascip = der;
      defaultPackage = der;
    });
}
