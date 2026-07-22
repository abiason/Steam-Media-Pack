#!/usr/bin/env bash
set -Eeuo pipefail

BASE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"
PACK_DIR="$DATA_HOME/steam-media-pack-11"
APP_DIR="$DATA_HOME/applications"
ICON_DIR="$DATA_HOME/icons/hicolor/scalable/apps"

mkdir -p "$PACK_DIR/launchers" "$APP_DIR" "$ICON_DIR" "$CONFIG_HOME/steam-media-pack-11"

install -m 755 "$BASE/launchers/"*.sh "$PACK_DIR/launchers/"
install -m 755 "$BASE/diagnostico.sh" "$PACK_DIR/diagnostico.sh"
install -m 755 "$BASE/selecionar-modo.sh" "$PACK_DIR/selecionar-modo.sh"
install -m 644 "$BASE/desktop/"*.desktop "$APP_DIR/"
install -m 644 "$BASE/icons/"*.svg "$ICON_DIR/"

cat >"$CONFIG_HOME/steam-media-pack-11/mode.conf" <<'EOF'
MODE=auto
EOF

command -v update-desktop-database >/dev/null 2>&1 && update-desktop-database "$APP_DIR" || true

cat <<EOF

Steam Media Pack 11.0 instalado em:
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
