#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="disney-plus"
APP_NAME="Disney+"
APP_URL="https://www.disneyplus.com/pt-br"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
