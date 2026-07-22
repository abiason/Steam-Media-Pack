#!/usr/bin/env bash
set -Eeuo pipefail

DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"

rm -rf "$DATA_HOME/steam-media-pack-11"
rm -rf "$CONFIG_HOME/steam-media-pack-11"
rm -f "$DATA_HOME/applications"/steam-media-*.desktop
rm -f "$DATA_HOME/icons/hicolor/scalable/apps"/steam-media-*.svg

echo "Steam Media Pack 11.0 removido."
