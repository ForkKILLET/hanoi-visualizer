with import <nixpkgs> {};
mkShell {
  name = "gcc-shell";

  buildInputs = [
    raylib
    jq
  ];

  shellHook = import ./shell-hook.nix;
}