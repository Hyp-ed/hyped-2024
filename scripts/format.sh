#!/bin/sh
# Formats all C++ and CMakeLists.txt files in the repository
# Requires git-clang-format and cmake-format
# Usage: ./scripts/format.sh [path]
# Path is relative to the root of the repository, if not specified, the entire repository is formatted

path="$(git rev-parse --show-toplevel)"
git add "$path/$argv"
git clang-format --extensions c,h,cpp,hpp
find "$path/$argv" \( -name 'CMakeLists.txt' -not -path "./build/*" -not -path "./telemetry/*" \) -exec cmake-format -i {} +
git add "$path/$argv"