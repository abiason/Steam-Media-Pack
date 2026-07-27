#!/usr/bin/env bash
set -Eeuo pipefail

BASE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE_DIR="$BASE/controller-bridge"

for command_name in cc make pkg-config; do
    if ! command -v "$command_name" >/dev/null 2>&1; then
        printf 'Dependência ausente: %s\n' "$command_name" >&2
        exit 1
    fi
done

if ! pkg-config --exists sdl2; then
    echo 'SDL2 de desenvolvimento não encontrado pelo pkg-config.' >&2
    echo 'No CachyOS/Arch, instale os pacotes base-devel e sdl2.' >&2
    exit 1
fi

make -C "$SOURCE_DIR" clean all
