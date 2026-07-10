#!/usr/bin/env bash
# Run the same checks as GitHub Actions locally.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT}"

run_step() {
    echo
    echo "==> $1"
    shift
    "$@"
}

run_step "clang-format" ./scripts/ci_format.sh
run_step "build" ./scripts/ci_build.sh
run_step "cppcheck" ./scripts/ci_cppcheck.sh
run_step "clang-tidy" ./scripts/ci_clang_tidy.sh
run_step "unit tests" ./scripts/ci_test_unit.sh
run_step "python lint" ./scripts/lint_python.sh
run_step "integration smoke" ./scripts/ci_integration.sh

echo
echo "All CI checks passed."
