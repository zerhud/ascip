{
  description = "cpp parser inspired with boost.spirit, without dependencies and poison";
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    cppbm.url = "github:zerhud/cppbm";
    cppbm.inputs.nixpkgs.follows = "nixpkgs";
  };
  outputs = params@{ self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; config.allowUnfree = true; };
      tref = params.cppbm.packages."${system}".tref;
      der = pkgs.gcc14Stdenv.mkDerivation {
        name = "ascip";
        propagatedBuildInputs = [ tref ];
        nativeBuildInputs = [pkgs.clang_19];
        installPhase = "mkdir -p \"$out/include\" && cp ascip.hpp -t \"$out/include\" && cp -rt \"$out/include\" ascip";
        buildPhase = "g++ -std=c++23 -fwhole-program -march=native -I. ./tests/in_parsers.cpp -o ascip_test && ./ascip_test";
        meta.description = "ascip is a clean cpp ascii parser, it doesn't requires any changes in using code, has no dependecies. single header file.";
        src = ./.;
      };
    in rec {
      devShell = der.overrideAttrs(finalAttrs: previousAttrs: {
          nativeBuildInputs =  previousAttrs.nativeBuildInputs ++ [ pkgs.jetbrains.clion ];
        });
      packages.default = der;
      packages.ascip = der;
      defaultPackage = der;
    });
}
