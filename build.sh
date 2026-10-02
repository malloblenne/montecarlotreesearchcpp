#!/usr/bin/env bash
set -euo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
mkdir -p "$root/build"
cd "$root/build"

cmake .. "$@"
cores=$(getconf _NPROCESSORS_ONLN)
make -j"$cores"