#!/usr/bin/env bash
set -Eeuo pipefail

BASE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Funções compartilhadas.
source "$BASE/scripts/common.sh"

DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"

PACK_DIR="$DATA_HOME/steam-media-pack"
LAUNCHER_INSTALL_DIR="$PACK_DIR/launchers"
INSTALLED_BIN_DIR="$PACK_DIR/bin"
PACK_CONFIG_DIR="$PACK_DIR/config"

APP_DIR="$DATA_HOME/applications"
ICON_DIR="$DATA_HOME/icons/hicolor/scalable/apps"
MODE_CONFIG="$CONFIG_HOME/steam-media-pack/mode.conf"

CONTROLLER_BRIDGE_DIR="$BASE/controller-bridge"
CONTROLLER_BRIDGE_BIN="$CONTROLLER_BRIDGE_DIR/controller-bridge"
INSTALLED_CONTROLLER_BRIDGE="$INSTALLED_BIN_DIR/controller-bridge"

CONTROLLER_BRIDGE_CONFIG_SOURCE="$CONTROLLER_BRIDGE_DIR/controller.conf.example"
CONTROLLER_BRIDGE_CONFIG_TARGET="$PACK_CONFIG_DIR/controller.conf"

SYSTEMD_INSTALL_SCRIPT="$BASE/packaging/systemd/install-controller-bridge-service.sh"
STEAM_SHORTCUT_INSTALLER="$BASE/tools/install-steam-shortcuts.sh"

on_error()
{
    local exit_code=$?
    local line_number="${1:-desconhecida}"

    log_error "A instalação falhou na linha $line_number."
    exit "$exit_code"
}

trap 'on_error "$LINENO"' ERR

require_command()
{
    local command_name="$1"
    local package_hint="${2:-$1}"

    if ! command_exists "$command_name"; then
        log_error "Dependência não encontrada: $command_name"
        log_error "Instale o pacote correspondente: $package_hint"
        exit 1
    fi
}

install_project_files()
{
    log_info "Criando diretórios de instalação..."

    ensure_directory "$LAUNCHER_INSTALL_DIR"
    ensure_directory "$INSTALLED_BIN_DIR"
    ensure_directory "$PACK_CONFIG_DIR"
    ensure_directory "$APP_DIR"
    ensure_directory "$ICON_DIR"
    ensure_directory "$(dirname "$MODE_CONFIG")"

    log_success "Diretórios preparados."

    log_info "Instalando launchers..."

    local launcher_files=("$BASE"/launchers/*.sh)

    if [[ ! -e "${launcher_files[0]}" ]]; then
        log_error "Nenhum launcher encontrado em: $BASE/launchers"
        exit 1
    fi

    install -m 755 \
        "$BASE"/launchers/*.sh \
        "$LAUNCHER_INSTALL_DIR/"

    install_executable \
        "$BASE/diagnostics.sh" \
        "$PACK_DIR/diagnostics.sh"

    install_executable \
        "$BASE/select-mode.sh" \
        "$PACK_DIR/select-mode.sh"

    log_success "Launchers instalados em: $LAUNCHER_INSTALL_DIR"

    log_info "Instalando atalhos do ambiente gráfico..."

    local desktop_files=("$BASE"/desktop/*.desktop)
    local icon_files=("$BASE"/icons/*.svg)

    if [[ -e "${desktop_files[0]}" ]]; then
        install -m 644 \
            "$BASE"/desktop/*.desktop \
            "$APP_DIR/"
    else
        log_warning "Nenhum arquivo .desktop encontrado."
    fi

    if [[ -e "${icon_files[0]}" ]]; then
        install -m 644 \
            "$BASE"/icons/*.svg \
            "$ICON_DIR/"
    else
        log_warning "Nenhum ícone SVG encontrado."
    fi

    if command_exists update-desktop-database; then
        update-desktop-database "$APP_DIR" || true
    fi

    log_success "Integração com o ambiente gráfico concluída."
}

install_mode_configuration()
{
    log_info "Preparando a configuração de modo..."

    if [[ ! -f "$MODE_CONFIG" ]]; then
        cat >"$MODE_CONFIG" <<'EOF'
MODE=auto
EOF

        log_success "Configuração padrão criada: $MODE_CONFIG"
    else
        log_info "Configuração existente preservada: $MODE_CONFIG"
    fi
}

build_controller_bridge()
{
    log_info "Compilando o Controller Bridge..."

    if [[ ! -f "$CONTROLLER_BRIDGE_DIR/Makefile" ]]; then
        log_error "Makefile não encontrado em: $CONTROLLER_BRIDGE_DIR"
        exit 1
    fi

    make -C "$CONTROLLER_BRIDGE_DIR"

    if [[ ! -x "$CONTROLLER_BRIDGE_BIN" ]]; then
        log_error "Binário não foi gerado: $CONTROLLER_BRIDGE_BIN"
        exit 1
    fi

    log_success "Controller Bridge compilado."
}

install_controller_bridge()
{
    log_info "Instalando o Controller Bridge..."

    install_executable \
        "$CONTROLLER_BRIDGE_BIN" \
        "$INSTALLED_CONTROLLER_BRIDGE"

    log_success \
        "Controller Bridge instalado em: $INSTALLED_CONTROLLER_BRIDGE"
}

install_controller_bridge_configuration()
{
    log_info "Preparando a configuração do Controller Bridge..."

    if [[ ! -f "$CONTROLLER_BRIDGE_CONFIG_SOURCE" ]]; then
        log_error \
            "Configuração de exemplo não encontrada: $CONTROLLER_BRIDGE_CONFIG_SOURCE"
        exit 1
    fi

    if [[ ! -f "$CONTROLLER_BRIDGE_CONFIG_TARGET" ]]; then
        install_regular_file \
            "$CONTROLLER_BRIDGE_CONFIG_SOURCE" \
            "$CONTROLLER_BRIDGE_CONFIG_TARGET"

        log_success \
            "Configuração criada em: $CONTROLLER_BRIDGE_CONFIG_TARGET"
    else
        log_info \
            "Configuração existente preservada: $CONTROLLER_BRIDGE_CONFIG_TARGET"
    fi
}

install_controller_bridge_service()
{
    log_info "Instalando o serviço do Controller Bridge..."

    if [[ ! -f "$SYSTEMD_INSTALL_SCRIPT" ]]; then
        log_warning \
            "Instalador do serviço não encontrado: $SYSTEMD_INSTALL_SCRIPT"
        return
    fi

    if ! command_exists systemctl; then
        log_warning \
            "systemctl não encontrado. O serviço não será instalado automaticamente."
        return
    fi

    bash "$SYSTEMD_INSTALL_SCRIPT"

    log_success "Serviço do Controller Bridge instalado e iniciado."
}

install_steam_shortcuts()
{
    log_info "Instalando atalhos e artes na Steam..."

    if [[ ! -f "$STEAM_SHORTCUT_INSTALLER" ]]; then
        log_error \
            "Instalador de atalhos não encontrado: $STEAM_SHORTCUT_INSTALLER"
        exit 1
    fi

    if pgrep -x steam >/dev/null 2>&1 ||
       pgrep -x steamwebhelper >/dev/null 2>&1; then
        log_error "A Steam está em execução."
        log_error "Feche completamente a Steam e execute o instalador novamente."
        exit 1
    fi

    bash "$STEAM_SHORTCUT_INSTALLER"

    log_success "Atalhos e artes da Steam instalados."
}

show_summary()
{
    cat <<EOF

============================================================
 Steam Media Pack instalado com sucesso
============================================================

Arquivos instalados em:
  $PACK_DIR

Launchers:
  $LAUNCHER_INSTALL_DIR

Controller Bridge:
  $INSTALLED_CONTROLLER_BRIDGE

Configuração do Controller Bridge:
  $CONTROLLER_BRIDGE_CONFIG_TARGET

Configuração de modo:
  $MODE_CONFIG

Os atalhos e as artes foram instalados nos usuários Steam
selecionados durante a instalação.

Abra novamente a Steam para visualizar:

  • Netflix
  • Prime Video
  • Disney+
  • Max
  • YouTube TV
  • Spotify

Modo automático:

  Desktop Mode -> app
  Gaming Mode  -> kiosk em tela cheia

EOF
}

main()
{
    log_info "Iniciando a instalação do Steam Media Pack..."

    require_command bash
    require_command install coreutils
    require_command make make
    require_command python3 python
    require_command pgrep procps-ng

    install_project_files
    install_mode_configuration

    build_controller_bridge
    install_controller_bridge
    install_controller_bridge_configuration
    install_controller_bridge_service

    install_steam_shortcuts

    show_summary
}

main "$@"
