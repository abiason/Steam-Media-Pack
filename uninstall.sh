#!/usr/bin/env bash

set -Eeuo pipefail

BASE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

APP_NAME="Steam Media Pack"
INSTALL_DIR="$HOME/.local/share/steam-media-pack"
CONFIG_DIR="$HOME/.config/steam-media-pack"
APPLICATIONS_DIR="$HOME/.local/share/applications"
SYSTEMD_USER_DIR="$HOME/.config/systemd/user"

SHORTCUT_REMOVER="$BASE/tools/remove-steam-shortcuts.py"
SERVICES_LIBRARY="$BASE/tools/lib/services.py"

CONTROLLER_SERVICE="controller-bridge.service"
CONTROLLER_SERVICE_PATH="$SYSTEMD_USER_DIR/$CONTROLLER_SERVICE"

ASSUME_YES=false
ALL_USERS=false
REMOVE_CONFIG=false

SHORTCUTS_REMOVED=false
SERVICE_REMOVED=false
FILES_REMOVED=false
CONFIG_REMOVED=false


info() {
    printf '[INFO] %s\n' "$*"
}


warning() {
    printf '[AVISO] %s\n' "$*" >&2
}


error() {
    printf '[ERRO] %s\n' "$*" >&2
}


show_help() {
    cat <<EOF
Uso:

  ./uninstall.sh [opções]

Opções:

  --yes             Confirma a desinstalação sem perguntar.
  --all-users       Remove os atalhos de todos os usuários Steam.
  --remove-config   Remove também as configurações do usuário.
  -h, --help        Mostra esta ajuda.

Exemplos:

  ./uninstall.sh
  ./uninstall.sh --yes --all-users
  ./uninstall.sh --yes --all-users --remove-config
EOF
}


on_error() {
    local exit_code=$?
    local line_number=${1:-desconhecida}

    error "A desinstalação falhou na linha ${line_number}."
    exit "$exit_code"
}


trap 'on_error "$LINENO"' ERR


parse_arguments() {
    while (($# > 0)); do
        case "$1" in
            --yes)
                ASSUME_YES=true
                ;;

            --all-users)
                ALL_USERS=true
                ;;

            --remove-config)
                REMOVE_CONFIG=true
                ;;

            -h|--help)
                show_help
                exit 0
                ;;

            *)
                error "Opção desconhecida: $1"
                echo
                show_help
                exit 2
                ;;
        esac

        shift
    done
}


confirm_uninstallation() {
    if [[ "$ASSUME_YES" == true ]]; then
        return
    fi

    local answer

    read -r -p \
        "Deseja realmente remover o Steam Media Pack? [s/N] " \
        answer

    case "${answer,,}" in
        s|sim|y|yes)
            ;;

        *)
            info "Desinstalação cancelada."
            exit 0
            ;;
    esac
}


confirm_config_removal() {
    if [[ "$REMOVE_CONFIG" == true ]]; then
        return
    fi

    if [[ "$ASSUME_YES" == true ]]; then
        return
    fi

    local answer

    echo
    read -r -p \
        "Deseja remover também as configurações? [s/N] " \
        answer

    case "${answer,,}" in
        s|sim|y|yes)
            REMOVE_CONFIG=true
            ;;
    esac
}


check_dependencies() {
    if ! command -v python3 >/dev/null 2>&1; then
        error "Python 3 não foi encontrado."
        exit 1
    fi

    if ! command -v systemctl >/dev/null 2>&1; then
        warning \
            "systemctl não foi encontrado. " \
            "A remoção do serviço será limitada."
    fi
}


check_steam_closed() {
    if pgrep -x steam >/dev/null 2>&1; then
        error "A Steam está em execução."
        error "Feche completamente a Steam antes de continuar."
        exit 1
    fi
}


get_service_ids() {
    if [[ ! -f "$SERVICES_LIBRARY" ]]; then
        warning \
            "Biblioteca comum de serviços não encontrada: " \
            "$SERVICES_LIBRARY"
        return 0
    fi

    PYTHONPATH="$BASE/tools${PYTHONPATH:+:$PYTHONPATH}" \
        python3 - <<'PY'
from lib.services import SERVICES

for service_id in SERVICES:
    print(service_id)
PY
}


remove_steam_shortcuts() {
    echo
    info "Removendo atalhos e artes da Steam..."

    if [[ ! -f "$SHORTCUT_REMOVER" ]]; then
        warning \
            "Removedor de atalhos não encontrado: " \
            "$SHORTCUT_REMOVER"
        return
    fi

    local command=(
        python3
        "$SHORTCUT_REMOVER"
    )

    if [[ "$ALL_USERS" == true ]]; then
        command+=(--all-users)
    fi

    if "${command[@]}"; then
        SHORTCUTS_REMOVED=true
    else
        error "Não foi possível remover os atalhos da Steam."
        exit 1
    fi
}


remove_controller_service() {
    echo
    info "Removendo o serviço Controller Bridge..."

    if command -v systemctl >/dev/null 2>&1; then
        systemctl --user stop \
            "$CONTROLLER_SERVICE" \
            >/dev/null 2>&1 || true

        systemctl --user disable \
            "$CONTROLLER_SERVICE" \
            >/dev/null 2>&1 || true
    fi

    rm -f -- "$CONTROLLER_SERVICE_PATH"

    if command -v systemctl >/dev/null 2>&1; then
        systemctl --user daemon-reload \
            >/dev/null 2>&1 || true

        systemctl --user reset-failed \
            "$CONTROLLER_SERVICE" \
            >/dev/null 2>&1 || true
    fi

    SERVICE_REMOVED=true
}


remove_service_desktop_entries() {
    local service_id
    local removed=0

    while IFS= read -r service_id; do
        [[ -n "$service_id" ]] || continue

        local candidates=(
            "$APPLICATIONS_DIR/${service_id}.desktop"
            "$APPLICATIONS_DIR/steam-media-pack-${service_id}.desktop"
        )

        local desktop_file

        for desktop_file in "${candidates[@]}"; do
            if [[ -f "$desktop_file" ]]; then
                rm -f -- "$desktop_file"
                ((removed += 1))
            fi
        done
    done < <(get_service_ids)

    info "$removed entrada(s) de aplicativo removida(s)."
}


remove_legacy_desktop_entries() {
    local legacy_files=(
        "$APPLICATIONS_DIR/netflix.desktop"
        "$APPLICATIONS_DIR/prime-video.desktop"
        "$APPLICATIONS_DIR/disney-plus.desktop"
        "$APPLICATIONS_DIR/max.desktop"
        "$APPLICATIONS_DIR/youtube-tv.desktop"
        "$APPLICATIONS_DIR/spotify.desktop"
    )

    rm -f -- "${legacy_files[@]}"
}


remove_icons() {
    local icon_directories=(
        "$HOME/.local/share/icons/steam-media-pack"
        "$HOME/.local/share/icons/hicolor/scalable/apps/steam-media-pack"
    )

    rm -rf -- "${icon_directories[@]}"

    local service_id

    while IFS= read -r service_id; do
        [[ -n "$service_id" ]] || continue

        rm -f -- \
            "$HOME/.local/share/icons/hicolor/"*/apps/"${service_id}.png" \
            "$HOME/.local/share/icons/hicolor/"*/apps/"steam-media-pack-${service_id}.png" \
            "$HOME/.local/share/icons/hicolor/scalable/apps/${service_id}.svg" \
            "$HOME/.local/share/icons/hicolor/scalable/apps/steam-media-pack-${service_id}.svg" \
            2>/dev/null || true
    done < <(get_service_ids)
}


remove_installed_files() {
    echo
    info "Removendo arquivos instalados..."

    remove_service_desktop_entries
    remove_legacy_desktop_entries
    remove_icons

    rm -rf -- "$INSTALL_DIR"

    rm -f -- \
        "$HOME/.local/bin/controller-bridge" \
        "$HOME/.local/bin/steam-media-pack" \
        "$HOME/.local/bin/steam-media-pack-controller-bridge"

    if command -v update-desktop-database >/dev/null 2>&1; then
        update-desktop-database \
            "$APPLICATIONS_DIR" \
            >/dev/null 2>&1 || true
    fi

    if command -v gtk-update-icon-cache >/dev/null 2>&1; then
        gtk-update-icon-cache \
            -f \
            -t \
            "$HOME/.local/share/icons/hicolor" \
            >/dev/null 2>&1 || true
    fi

    FILES_REMOVED=true
}


remove_configuration() {
    confirm_config_removal

    if [[ "$REMOVE_CONFIG" != true ]]; then
        info "As configurações foram preservadas em: $CONFIG_DIR"
        return
    fi

    echo
    info "Removendo configurações..."

    rm -rf -- "$CONFIG_DIR"

    CONFIG_REMOVED=true
}


print_summary() {
    echo
    echo "=============================================="
    echo " Desinstalação concluída"
    echo "=============================================="
    echo

    if [[ "$SHORTCUTS_REMOVED" == true ]]; then
        echo "  ✓ Atalhos e artes da Steam processados"
    else
        echo "  - Atalhos da Steam não foram encontrados"
    fi

    if [[ "$SERVICE_REMOVED" == true ]]; then
        echo "  ✓ Serviço Controller Bridge removido"
    fi

    if [[ "$FILES_REMOVED" == true ]]; then
        echo "  ✓ Arquivos do Steam Media Pack removidos"
    fi

    if [[ "$CONFIG_REMOVED" == true ]]; then
        echo "  ✓ Configurações removidas"
    else
        echo "  - Configurações preservadas"
    fi

    echo
}


main() {
    parse_arguments "$@"

    echo
    echo "=============================================="
    echo " ${APP_NAME} — Desinstalador"
    echo "=============================================="
    echo

    confirm_uninstallation
    check_dependencies
    check_steam_closed

    remove_steam_shortcuts
    remove_controller_service
    remove_installed_files
    remove_configuration

    print_summary
}


main "$@"
