#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="max"
APP_NAME="Max"
APP_URL="https://www.max.com/br/pt"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
