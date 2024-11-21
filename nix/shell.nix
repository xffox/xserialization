{ pkgs, xserialization }:
pkgs.mkShell {
  inputsFrom = [ xserialization ];
  packages = with pkgs; [
    pkgs.clang-tools
    pkgs.aflplusplus
    pkgs.valgrind
  ];
}
