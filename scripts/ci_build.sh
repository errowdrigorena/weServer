#!/usr/bin/env bash
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

ensure_certs
configure_build
build_project

echo "Build: OK"
