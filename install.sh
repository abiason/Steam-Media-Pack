#!/usr/bin/env bash
set -Eeuo pipefail

BASE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Funções compartilhadas do instalador.
source "$BASE/scripts/common.sh"

DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
CONFIG_HOME="${XDG_CONFIG_HOME:-$HOME/.config}"
PACK_DIR="$DATA_HOME/steam-media-pack"
APP_DIR="$DATA_HOME/applications"
ICON_DIR="$DATA_HOME/icons/hicolor/scalable/apps"
CONTROLLER_BRIDGE_DIR="$BASE/controller-bridge"
CONTROLLER_BRIDGE_BIN="$CONTROLLER_BRIDGE_DIR/controller-bridge"
INSTALLED_BIN_DIR="$PACK_DIR/bin"
INSTALLED_CONTROLLER_BRIDGE="$INSTALLED_BIN_DIR/controller-bridge"
CONTROLLER_BRIDGE_CONFIG_SOURCE="$CONTROLLER_BRIDGE_DIR/controller.conf.example"
CONTROLLER_BRIDGE_CONFIG_DIR="$PACK_DIR/config"
CONTROLLER_BRIDGE_CONFIG_TARGET="$CONTROLLER_BRIDGE_CONFIG_DIR/controller.conf"
SYSTEMD_DIR="$BASE/packaging/systemd"
SYSTEMD_INSTALL_SCRIPT="$SYSTEMD_DIR/install-controller-bridge-service.sh"

log_info "Criando diretórios de instalação..."

ensure_directory "$PACK_DIR/launchers"
ensure_directory "$INSTALLED_BIN_DIR"
ensure_directory "$APP_DIR"
ensure_directory "$ICON_DIR"
ensure_directory "$CONFIG_HOME/steam-media-pack"
ensure_directory "$CONTROLLER_BRIDGE_CONFIG_DIR"

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

log_info "Instalando o Controller Bridge..."

install_executable \
    "$CONTROLLER_BRIDGE_BIN" \
    "$INSTALLED_CONTROLLER_BRIDGE"

log_success "Controller Bridge instalado em: $INSTALLED_CONTROLLER_BRIDGE"

log_info "Preparando a configuração do Controller Bridge..."

if [[ ! -f "$CONTROLLER_BRIDGE_CONFIG_SOURCE" ]]; then
    log_error "Configuração de exemplo não encontrada: $CONTROLLER_BRIDGE_CONFIG_SOURCE"
    exit 1
fi

if [[ ! -f "$CONTROLLER_BRIDGE_CONFIG_TARGET" ]]; then
    install_regular_file \
        "$CONTROLLER_BRIDGE_CONFIG_SOURCE" \
        "$CONTROLLER_BRIDGE_CONFIG_TARGET"

    log_success "Configuração criada em: $CONTROLLER_BRIDGE_CONFIG_TARGET"
else
    log_info "Configuração existente preservada: $CONTROLLER_BRIDGE_CONFIG_TARGET"
fi

log_success "Diretórios preparados."

install -m 755 "$BASE/launchers/"*.sh "$PACK_DIR/launchers/"
install -m 755 "$BASE/diagnostics.sh" "$PACK_DIR/diagnostics.sh"
install -m 755 "$BASE/select-mode.sh" "$PACK_DIR/select-mode.sh"
install -m 644 "$BASE/desktop/"*.desktop "$APP_DIR/"
install -m 644 "$BASE/icons/"*.svg "$ICON_DIR/"

MODE_CONFIG="$CONFIG_HOME/steam-media-pack/mode.conf"

if [[ ! -f "$MODE_CONFIG" ]]; then
    cat >"$MODE_CONFIG" <<'EOF'
MODE=auto
EOF

    log_success "Configuração padrão criada: $MODE_CONFIG"
else
    log_info "Configuração existente preservada: $MODE_CONFIG"
fi

if command -v update-desktop-database >/dev/null 2>&1; then
    update-desktop-database "$APP_DIR" || true
fi

log_info "Instalando o serviço do Controller Bridge..."

if [[ -x "$SYSTEMD_INSTALL_SCRIPT" ]]; then
    "$SYSTEMD_INSTALL_SCRIPT"
    log_success "Serviço instalado."
else
    log_warning "Instalador do serviço não encontrado."
fi

cat <<EOF

Steam Media Pack instalado em:
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
