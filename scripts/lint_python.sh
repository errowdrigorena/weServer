#!/usr/bin/env bash
# Broad, low-friction checks for the Python helper scripts.
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

mapfile -t scripts < <(find "${ROOT}/scripts" -maxdepth 1 -name '*.py' -print | sort)

if [[ ${#scripts[@]} -eq 0 ]]; then
    echo "No Python scripts found."
    exit 0
fi

echo "Python syntax check (py_compile)..."
python3 -m py_compile "${scripts[@]}"

if command -v ruff >/dev/null 2>&1; then
    echo "Running ruff (broad lint)..."
    ruff check "${scripts[@]}"
else
    echo "ruff not installed; running stdlib compileall..."
    python3 -m compileall -q "${ROOT}/scripts"
fi

echo "Python lint: OK"
