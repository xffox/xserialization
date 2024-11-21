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
        defaultWithSample = targetPkg { enableSample = true; };
        withoutJSON = targetPkg { useJSON = false; };
        buildClang = targetPkg { stdenv = pkgs.clangStdenv; };
        buildStatic = targetPkg { stdenv = pkgs.pkgsStatic.stdenv; buildSharedLibs = false; };
        buildStaticWithSample = targetPkg { stdenv = pkgs.pkgsStatic.stdenv; buildSharedLibs = false; enableSample = true; };
      };
      devShells = {
        default = pkgs.callPackage ./nix/shell.nix { xserialization = self.packages.${system}.default; };
      };
    });
}
