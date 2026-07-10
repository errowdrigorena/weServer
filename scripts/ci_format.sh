#!/usr/bin/env bash
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"

if ! command -v "${CLANG_FORMAT}" >/dev/null 2>&1; then
    echo "error: ${CLANG_FORMAT} not found" >&2
    exit 1
fi

mapfile -t sources < <(find_sources)

if [[ ${#sources[@]} -eq 0 ]]; then
    echo "No C++ sources found for formatting check."
    exit 0
fi

echo "Checking formatting with ${CLANG_FORMAT}..."
"${CLANG_FORMAT}" --dry-run --Werror "${sources[@]}"
echo "clang-format: OK"
