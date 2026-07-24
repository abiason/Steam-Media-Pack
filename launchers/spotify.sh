#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="spotify"
APP_NAME="Spotify"
APP_URL="https://open.spotify.com/"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
