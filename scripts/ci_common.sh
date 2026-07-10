#!/usr/bin/env bash
# Shared helpers for local CI scripts and GitHub Actions.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${WESERVER_BUILD_DIR:-${ROOT}/build/Debug}"
BUILD_TYPE="${WESERVER_BUILD_TYPE:-Debug}"
CPP_STD="${WESERVER_CPP_STD:-20}"

cd "${ROOT}"

ensure_certs() {
    if [[ -f server.cert.pem && -f server.key.pem ]]; then
        return 0
    fi

    echo "Generating self-signed certificate for CI..."
    openssl req -x509 -newkey rsa:2048 -nodes \
        -keyout server.key.pem \
        -out server.cert.pem \
        -days 3650 \
        -subj "/C=ES/ST=Madrid/L=Madrid/O=weServer/CN=localhost"
}

configure_build() {
    conan install . \
        --build=missing \
        -s compiler.cppstd="${CPP_STD}" \
        -s build_type="${BUILD_TYPE}"

  if [[ "${BUILD_TYPE}" == "Debug" ]]; then
      cmake --preset conan-debug
  else
      cmake --preset "conan-${BUILD_TYPE,,}"
  fi
}

build_project() {
    if [[ "${BUILD_TYPE}" == "Debug" ]]; then
        cmake --build --preset conan-debug
    else
        cmake --build --preset "conan-${BUILD_TYPE,,}"
    fi
}

find_sources() {
    find include src test \
        \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
        -print
}

is_in_compile_commands() {
    local source="$1"
    local abs_source
    abs_source="$(realpath "${source}")"
    grep -Fq "\"file\": \"${abs_source}\"" "${BUILD_DIR}/compile_commands.json"
}

core_tidy_compile_flags() {
    printf '%s\n' \
        -std=c++20 \
        "-I${ROOT}/include" \
        "-I${ROOT}/include/Core" \
        -DBOOST_BEAST_USE_STD_STRING_VIEW
}
