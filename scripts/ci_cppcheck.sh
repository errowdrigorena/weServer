#!/usr/bin/env bash
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

CPPCHECK="${CPPCHECK:-cppcheck}"
COMPILE_DB="${BUILD_DIR}/compile_commands.json"

if ! command -v "${CPPCHECK}" >/dev/null 2>&1; then
    echo "error: ${CPPCHECK} not found" >&2
    exit 1
fi

if [[ ! -f "${COMPILE_DB}" ]]; then
    echo "compile_commands.json not found; configuring build first..."
    configure_build
fi

common_cppcheck_args=(
    --enable=warning,style,performance,portability
    --inline-suppr
    --suppressions-list="${ROOT}/cppcheck.suppressions"
    --std=c++20
    --error-exitcode=1
)

echo "Running cppcheck (CMake targets)..."
"${CPPCHECK}" \
    --project="${COMPILE_DB}" \
    --library=googletest \
    "${common_cppcheck_args[@]}"

mapfile -t standalone_sources < <(
    while IFS= read -r source; do
        if [[ "${source}" == *.cpp ]] && ! is_in_compile_commands "${source}"; then
            printf '%s\n' "${source}"
        fi
    done < <(find_sources)
)

if [[ ${#standalone_sources[@]} -gt 0 ]]; then
    echo "Running cppcheck (Core / standalone sources)..."
    "${CPPCHECK}" \
        "${common_cppcheck_args[@]}" \
        -I"${ROOT}/include" \
        -I"${ROOT}/include/Core" \
        "${standalone_sources[@]}"
fi

echo "cppcheck: OK"
