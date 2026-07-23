#!/usr/bin/env bash
set -Eeuo pipefail

BASE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"
PACK_DIR="$DATA_HOME/steam-media-pack"
APP_DIR="$DATA_HOME/applications"
ICON_DIR="$DATA_HOME/icons/hicolor/scalable/apps"

mkdir -p "$PACK_DIR/launchers" "$APP_DIR" "$ICON_DIR" "$CONFIG_HOME/steam-media-pack"

install -m 755 "$BASE/launchers/"*.sh "$PACK_DIR/launchers/"
install -m 755 "$BASE/diagnostics.sh" "$PACK_DIR/diagnostics.sh"
install -m 755 "$BASE/select-mode.sh" "$PACK_DIR/select-mode.sh"
install -m 644 "$BASE/desktop/"*.desktop "$APP_DIR/"
install -m 644 "$BASE/icons/"*.svg "$ICON_DIR/"

cat >"$CONFIG_HOME/steam-media-pack/mode.conf" <<'EOF'
MODE=auto
EOF

if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "$APP_DIR" || true
fi

cat <<EOF

Steam Media Pack instalado em:
  $PACK_DIR

Netflix na Steam:

Destino:
  /usr/bin/bash

Iniciar em:
  $PACK_DIR/launchers

Opções de inicialização:
  "$PACK_DIR/launchers/netflix.sh"

Compatibilidade Proton:
  Desmarcada

Modo automático:
  Desktop Mode -> app
  Gaming Mode -> kiosk em tela cheia
EOF
