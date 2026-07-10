#!/usr/bin/env bash
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

if [[ ! -d "${BUILD_DIR}" ]]; then
    echo "Build directory missing; building first..."
    "$(dirname "$0")/ci_build.sh"
fi

echo "Running unit tests..."
ctest --test-dir "${BUILD_DIR}" --output-on-failure
echo "Unit tests: OK"
