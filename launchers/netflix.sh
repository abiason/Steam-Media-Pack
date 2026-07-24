#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="netflix"
APP_NAME="Netflix"
APP_URL="https://www.netflix.com/br/"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=common.sh
source "$SCRIPT_DIR/common.sh"
