#!/usr/bin/env bash
set -Eeuo pipefail

CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"
CONF="$CONFIG_HOME/steam-media-pack/mode.conf"
mkdir -p "$(dirname "$CONF")"

echo "Steam Media Pack — modo de abertura"
echo
echo "1) auto    - Desktop usa app; Gaming Mode usa kiosk"
echo "2) app     - janela sem barra de endereço"
echo "3) browser - navegador normal"
echo "4) kiosk   - tela cheia sem barra"
echo "5) xdg     - associação padrão do sistema"
echo
read -r -p "Escolha [1-5]: " choice

case "$choice" in
    1) mode=auto ;;
    2) mode=app ;;
    3) mode=browser ;;
    4) mode=kiosk ;;
    5) mode=xdg ;;
    *) echo "Opção inválida."; exit 1 ;;
esac

printf 'MODE=%s\n' "$mode" >"$CONF"
echo "Modo definido como: $mode"
