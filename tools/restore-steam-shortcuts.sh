#!/usr/bin/env bash
set -Eeuo pipefail
for root in "$HOME/.local/share/Steam" "$HOME/.steam/steam"; do
  [ -d "$root/userdata" ] || continue
  for cfg in "$root"/userdata/*/config; do
    [ -d "$cfg" ] || continue
    b="$cfg/shortcuts.vdf.steam-media-pack.backup"
    if [ -f "$b" ]; then
      cp -f "$b" "$cfg/shortcuts.vdf"
      echo "Backup restaurado: $cfg/shortcuts.vdf"
    fi
  done
done
