with import <nixpkgs> {};
mkShell {
  buildInputs = [
    raylib
    jq
  ];

  shellHook = let
    dollar = "$";
    flags = [
      "-I${dollar}{workspaceFolder}/inc"
      "-I${raylib}/include"
    ];
  in ''
    function write-vscode-config () {
      jq '.["clangd.fallbackFlags"] = ${builtins.toJSON flags}' ./.vscode/settings.json \
        | tee ./.vscode/settings.new.json \
        && mv ./.vscode/settings.new.json ./.vscode/settings.json
    }
  '';
}