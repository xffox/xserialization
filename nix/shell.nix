{ pkgs, stdenv, gdb, xserialization }:
pkgs.mkShell.override {inherit stdenv;} {
  inputsFrom = [ xserialization ];
  packages = with pkgs; [
    pkgs.clang-tools
    pkgs.aflplusplus
    pkgs.valgrind
    gdb
  ];
}
