{ stdenv, cmake, nlohmann_json, pkg-config, cppunit, ninja,
  buildSharedLibs ? true, useJSON ? true, enableSample ? false}:
stdenv.mkDerivation {
  name = "xserialization";
  src = ./..;

  nativeBuildInputs = [
    cmake
    ninja
    pkg-config
  ];
  buildInputs = [] ++
    (if useJSON then [nlohmann_json] else []);
  checkInputs = [
    cppunit
  ];

  doCheck = true;

  cmakeFlags = [] ++
  (if useJSON then ["-DUSE_JSON=on"] else []) ++
  (if buildSharedLibs then ["-DBUILD_SHARED_LIBS=on"] else []) ++
  (if enableSample then ["-DENABLE_SAMPLE=on"] else []);
}
