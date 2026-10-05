#!/usr/bin/env bash
set -euo pipefail

# Resolve paths from this script, independently of the calling directory.
project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

for tool in cmake ninja; do
    if ! command -v "$tool" >/dev/null 2>&1; then
        printf 'Error: %s is required. Install CMake, Ninja and a C compiler in WSL.\n' "$tool" >&2
        exit 1
    fi
done

if [[ ! -f "$project_dir/zenoh-pico/CMakeLists.txt" ]]; then
    printf 'Error: zenoh-pico is missing. Run git submodule update --init --recursive first.\n' >&2
    exit 1
fi

cd -- "$project_dir/App"
printf 'Configuring App (dev preset)...\n'
cmake --preset dev
printf 'Cleaning and rebuilding all App examples...\n'
cmake --build --preset dev --clean-first
printf 'Build complete: %s/build\n' "$project_dir"
