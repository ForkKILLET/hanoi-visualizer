with import <nixpkgs> {};
let
  dollar = "$";
  flags = [
    "-I${dollar}{workspaceFolder}/inc"
    "-I${raylib}/include"
  ];
in ''
  function @write-vscode-config () {
    jq '.["clangd.fallbackFlags"] = ${builtins.toJSON flags}' ./.vscode/settings.json \
      | tee ./.vscode/settings.new.json \
      && mv ./.vscode/settings.new.json ./.vscode/settings.json
  }

  function @core-dump-copy-latest () {
    local file=$(ls /var/lib/systemd/coredump/core.hanoi.* -t | head -n 1)
    if [ -n "$file" ]; then
      sudo cp "$file" ./debug
      echo "Copied latest core dump: $file"
      local basename=$(basename "$file")
      sudo chown $USER ./debug/$basename
      zstd -d ./debug/$basename
      echo "Decompressed core dump: ./debug/$(basename $basename .zst)"
      rm ./debug/$basename
    else
      echo "No core dumps found."
    fi
  }

  function @debug-latest() {
    local file=$(ls ./debug/core.hanoi.* -t | head -n 1)
    if [ -n "$file" ]; then
      gdb ./build/linux/x86_64/debug/hanoi "$file"
    else
      echo "No core dumps found in ./debug."
    fi
  }

  function @core-dump-clear () {
    ls ./debug
    rm -rf ./debug/*
    echo "Cleared all core dumps."
  }
''