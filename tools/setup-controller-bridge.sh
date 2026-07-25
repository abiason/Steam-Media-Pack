#!/usr/bin/env bash
set -Eeuo pipefail

DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
PACK_DIR="$DATA_HOME/steam-media-pack"
RULE_SOURCE="$PACK_DIR/controller-bridge/99-steam-media-controller-bridge.rules"
RULE_DEST="/etc/udev/rules.d/99-steam-media-controller-bridge.rules"
SERVICE_NAME="steam-media-controller-bridge.service"

if [[ ! -f "$RULE_SOURCE" ]]; then
    printf 'Regra udev não encontrada: %s\n' "$RULE_SOURCE" >&2
    exit 1
fi

if ! command -v sudo >/dev/null 2>&1; then
    echo "sudo não encontrado. Instale a regra udev manualmente em $RULE_DEST." >&2
    exit 1
fi

sudo install -Dm644 "$RULE_SOURCE" "$RULE_DEST"
sudo modprobe uinput
sudo udevadm control --reload-rules
sudo udevadm trigger --subsystem-match=misc --action=add

systemctl --user daemon-reload
systemctl --user enable --now "$SERVICE_NAME"

cat <<'MSG'

Controller Bridge configurado.

Controles:
  Analógico direito  -> mover cursor
  A                   -> clique esquerdo
  B                   -> clique direito
  X                   -> clique do meio
  Gatilho esquerdo    -> rolar para cima
  Gatilho direito     -> rolar para baixo

Confira o estado com:
  systemctl --user status steam-media-controller-bridge.service
MSG
