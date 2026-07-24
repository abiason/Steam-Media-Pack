#!/usr/bin/env bash
set -Eeuo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
exec "$SCRIPT_DIR/common.sh" 'disney-plus' 'Disney+' 'https://www.disneyplus.com/pt-br'
