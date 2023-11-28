{
  description = "xserialization";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }@inputs:
  flake-utils.lib.eachDefaultSystem (system :
    let pkgs = nixpkgs.legacyPackages.${system};
        targetPkg = pkgs.callPackage ./nix/build.nix;
    in {
      packages = {
        default = targetPkg {};
        withoutJSON = targetPkg { useJSON = false; };
        buildClang = targetPkg { stdenv = pkgs.clangStdenv; };
      };
      devShells = {
        default = pkgs.mkShell {
          packages = [
            pkgs.clang-tools
          ];
          inputsFrom = [self.packages.${system}.default];
        };
      };
    });
}
