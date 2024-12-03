{ pkgs, stdenv, xserialization }:
pkgs.mkShell.override {inherit stdenv;} {
  inputsFrom = [ xserialization ];
  packages = with pkgs; [
    pkgs.clang-tools
    pkgs.aflplusplus
    pkgs.valgrind
  ];
}
