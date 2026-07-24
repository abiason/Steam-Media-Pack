#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="youtube-tv"
APP_NAME="YouTube TV"
APP_URL="https://www.youtube.com/tv"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
