{ stdenv, cmake, nlohmann_json, pkg-config, cppunit,
  useJSON ? true}:
stdenv.mkDerivation {
  name = "xserialization";
  src = ./..;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];
  buildInputs = [] ++
    (if useJSON then [nlohmann_json] else []);
  checkInputs = [
    cppunit
  ];

  doCheck = true;

  cmakeFlags = [] ++
    (if useJSON then ["-DUSE_JSON=on"] else []);
}
