#!/usr/bin/env bash
set -euo pipefail

source "$(dirname "$0")/ci_common.sh"

SERVER_BIN="${BUILD_DIR}/src/weserver"
SERVER_PID=""

cleanup() {
    if [[ -n "${SERVER_PID}" ]] && kill -0 "${SERVER_PID}" 2>/dev/null; then
        kill "${SERVER_PID}" 2>/dev/null || true
        wait "${SERVER_PID}" 2>/dev/null || true
    fi
}
trap cleanup EXIT

if [[ ! -x "${SERVER_BIN}" ]]; then
    echo "Server binary missing; building first..."
    "$(dirname "$0")/ci_build.sh"
fi

ensure_certs

echo "Starting weserver for integration smoke test..."
"${SERVER_BIN}" &
SERVER_PID=$!

ready=0
for _ in $(seq 1 40); do
    if python3 - "${BASE:-https://127.0.0.1:8080/crud/items}" <<'PY'
import ssl
import sys
import urllib.request

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

req = urllib.request.Request(sys.argv[1], method="GET")
try:
    with urllib.request.urlopen(req, context=ctx, timeout=1) as resp:
        raise SystemExit(0 if resp.status == 200 else 1)
except Exception:
    raise SystemExit(1)
PY
    then
        ready=1
        break
    fi
    sleep 0.25
done

if [[ "${ready}" -ne 1 ]]; then
    echo "Server did not become ready in time." >&2
    exit 1
fi

echo "Running CRUD smoke test..."
python3 "${ROOT}/scripts/test_crud.py" --ci

echo "Integration smoke test: OK"
