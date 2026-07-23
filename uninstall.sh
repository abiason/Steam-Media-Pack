#!/usr/bin/env bash
set -Eeuo pipefail

DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"

rm -rf "$DATA_HOME/steam-media-pack"
rm -rf "$CONFIG_HOME/steam-media-pack"
rm -f "$DATA_HOME/applications"/steam-media-*.desktop
rm -f "$DATA_HOME/icons/hicolor/scalable/apps"/steam-media-*.svg

echo "Steam Media Pack removido."
