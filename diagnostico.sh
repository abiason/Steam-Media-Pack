#!/usr/bin/env bash
set +e

CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"
STATE_HOME="${XDG_STATE_HOME:-$HOME/.local/state}"
LOG_DIR="$STATE_HOME/steam-media-pack-11/logs"
CONF="$CONFIG_HOME/steam-media-pack-11/mode.conf"

echo "=== Steam Media Pack 11.0 — Diagnóstico ==="
echo "Data: $(date --iso-8601=seconds)"
echo
echo "--- Configuração ---"
cat "$CONF" 2>/dev/null || echo "MODE=auto"
echo
echo "--- Ambiente gráfico ---"
env | grep -E '^(DISPLAY|WAYLAND_DISPLAY|GAMESCOPE|SteamGamepadUI|XDG_|DBUS_SESSION_BUS_ADDRESS|PIPEWIRE|PULSE|GTK_|QT_|KDE_)=' | sort
echo
echo "--- Logs ---"
if compgen -G "$LOG_DIR/*.log" >/dev/null; then
    for f in "$LOG_DIR"/*.log; do
        echo "### $f"
        tail -n 160 "$f"
    done
else
    echo "Nenhum log encontrado."
fi
