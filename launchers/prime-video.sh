#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="prime-video"
APP_NAME="Prime Video"
APP_URL="https://www.primevideo.com/"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
