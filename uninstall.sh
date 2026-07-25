#!/usr/bin/env bash
set -Eeuo pipefail

DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"

echo "Removendo Controller Bridge..."

systemctl --user disable --now steam-media-controller-bridge.service 2>/dev/null || true

rm -f "$HOME/.config/systemd/user/steam-media-controller-bridge.service"

systemctl --user daemon-reload

if [ -f /etc/udev/rules.d/99-steam-media-controller-bridge.rules ]; then
    sudo rm -f /etc/udev/rules.d/99-steam-media-controller-bridge.rules
    sudo udevadm control --reload-rules
    sudo udevadm trigger
fi

rm -rf "$HOME/.local/share/steam-media-pack/controller-bridge"

rm -rf "$DATA_HOME/steam-media-pack"
rm -rf "$CONFIG_HOME/steam-media-pack"
rm -f "$DATA_HOME/applications"/steam-media-*.desktop
rm -f "$DATA_HOME/icons/hicolor/scalable/apps"/steam-media-*.svg

echo "Steam Media Pack removido."
