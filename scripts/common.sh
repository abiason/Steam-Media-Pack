#!/usr/bin/env bash

set -Eeuo pipefail

log_info()
{
    printf '[INFO] %s\n' "$1"
}

log_success()
{
    printf '[OK] %s\n' "$1"
}

log_warning()
{
    printf '[AVISO] %s\n' "$1" >&2
}

log_error()
{
    printf '[ERRO] %s\n' "$1" >&2
}

command_exists()
{
    command -v "$1" >/dev/null 2>&1
}

ensure_directory()
{
    local directory="$1"

    mkdir -p "$directory"
}

install_executable()
{
    local source_file="$1"
    local target_file="$2"

    install -Dm755 "$source_file" "$target_file"
}

install_regular_file()
{
    local source_file="$1"
    local target_file="$2"

    install -Dm644 "$source_file" "$target_file"
}
