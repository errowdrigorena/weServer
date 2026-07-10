#!/usr/bin/env bash
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
COMPILE_DB="${BUILD_DIR}/compile_commands.json"

if ! command -v "${CLANG_TIDY}" >/dev/null 2>&1; then
    echo "error: ${CLANG_TIDY} not found" >&2
    exit 1
fi

if [[ ! -f "${COMPILE_DB}" ]]; then
    echo "compile_commands.json not found; configuring build first..."
    configure_build
fi

mapfile -t sources < <(find_sources)

if [[ ${#sources[@]} -eq 0 ]]; then
    echo "No C++ sources found for clang-tidy."
    exit 0
fi

mapfile -t core_flags < <(core_tidy_compile_flags)

echo "Running clang-tidy (cppcoreguidelines-*)..."
failed=0
for source in "${sources[@]}"; do
    if is_in_compile_commands "${source}"; then
        if ! "${CLANG_TIDY}" -p "${BUILD_DIR}" "${source}"; then
            failed=1
        fi
    else
        echo "clang-tidy (standalone): ${source}"
        if ! "${CLANG_TIDY}" "${source}" -- "${core_flags[@]}"; then
            failed=1
        fi
    fi
done

if [[ "${failed}" -ne 0 ]]; then
    echo "clang-tidy: FAILED" >&2
    exit 1
fi

echo "clang-tidy: OK"
