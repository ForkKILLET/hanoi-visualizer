with import <nixpkgs> {};
clangStdenv.mkDerivation {
  name = "clang-shell";
  src = null;

  nativeBuildInputs = [
    clang
  ];

  buildInputs = [
    raylib
    jq
    clang
  ];

  shellHook = import ./shell-hook.nix;
}