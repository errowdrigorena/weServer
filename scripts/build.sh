#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

if [[ ! -f server.cert.pem || ! -f server.key.pem ]]; then
  echo "Generating self-signed certificate..."
  openssl req -x509 -newkey rsa:2048 -nodes \
    -keyout server.key.pem \
    -out server.cert.pem \
    -days 3650 \
    -subj "/C=ES/ST=Madrid/L=Madrid/O=weServer/CN=localhost"
fi

conan install . --build=missing -s compiler.cppstd=20 -s build_type=Debug
cmake --preset conan-debug
cmake --build --preset conan-debug

echo
echo "Build complete. Run: ./build/Debug/weserver"
