#!/usr/bin/env bash
set -Eeuo pipefail

APP_ID="${1:?APP_ID não definido}"
APP_NAME="${2:?APP_NAME não definido}"
APP_URL="${3:?APP_URL não definido}"

STATE_HOME="${XDG_STATE_HOME:-$HOME_DIR/.local/state}"
PROFILE_DIR="$CONFIG_HOME/steam-media-pack/profiles/$APP_ID"
LOG_DIR="$STATE_HOME/steam-media-pack/logs"
LOG_FILE="$LOG_DIR/$APP_ID.log"
PREF_FILE="$CONFIG_HOME/steam-media-pack/mode.conf"

mkdir -p "$PROFILE_DIR/Default" "$LOG_DIR" "$(dirname "$PREF_FILE")"
touch "$LOG_FILE"

log() {
    printf '[%s] %s\n' "$(date --iso-8601=seconds)" "$*" >>"$LOG_FILE"
}

is_gaming_mode() {
    [[ "${SteamGamepadUI:-0}" == "1" ]] || [[ -n "${GAMESCOPE_WAYLAND_DISPLAY:-}" ]]
}

find_native_browser() {
    local b
    for b in \
        google-chrome-stable google-chrome chromium \
        brave brave-browser vivaldi-stable vivaldi \
        microsoft-edge-stable; do
        if command -v "$b" >/dev/null 2>&1; then
            command -v "$b"
            return 0
        fi
    done
    return 1
}

find_flatpak_browser() {
    command -v flatpak >/dev/null 2>&1 || return 1
    local app
    for app in \
        com.google.Chrome org.chromium.Chromium com.brave.Browser \
        com.vivaldi.Vivaldi com.microsoft.Edge; do
        if flatpak info "$app" >/dev/null 2>&1; then
            printf '%s\n' "$app"
            return 0
        fi
    done
    return 1
}

sanitize_environment() {
    unset LD_PRELOAD LD_LIBRARY_PATH LD_AUDIT
    unset STEAM_RUNTIME STEAM_RUNTIME_HEAVY STEAM_RUNTIME_PREFER_HOST_LIBRARIES
    unset STEAM_COMPAT_DATA_PATH STEAM_COMPAT_CLIENT_INSTALL_PATH
    unset STEAM_COMPAT_LIBRARY_PATHS STEAM_COMPAT_MOUNTS STEAM_COMPAT_SHADER_PATH
    unset PRESSURE_VESSEL_FILESYSTEMS_RW PRESSURE_VESSEL_FILESYSTEMS_RO
    unset PRESSURE_VESSEL_WRAP PRESSURE_VESSEL_RUNTIME PRESSURE_VESSEL_RUNTIME_BASE
    unset PRESSURE_VESSEL_VARIABLE_DIR
    unset WINEPREFIX WINEESYNC WINEFSYNC WINEDLLOVERRIDES
    unset PROTON_LOG PROTON_DUMP_DEBUG_COMMANDS PROTON_REMOTE_DEBUG_CMD PROTON_REMOTE_DEBUG_DIR
    unset DXVK_LOG_LEVEL DXVK_CONFIG_FILE VKD3D_CONFIG VKD3D_DEBUG
    unset GAMEOVERLAYRENDERER ENABLE_VKBASALT MANGOHUD OBS_VKCAPTURE
    unset SDL_GAMECONTROLLER_IGNORE_DEVICES SDL_GAMECONTROLLERCONFIG
    unset SDL_JOYSTICK_HIDAPI

    local var
    while IFS='=' read -r var _; do
        case "$var" in
            STEAM_*|PRESSURE_VESSEL_*|PROTON_*|WINE*|DXVK_*|VKD3D_*|GAMEOVERLAY*|SteamOverlay*)
                unset "$var" || true
                ;;
        esac
    done < <(env)
}

patch_chrome_preferences() {
    command -v python3 >/dev/null 2>&1 || return 0

    PROFILE_DIR="$PROFILE_DIR" python3 <<'PY' >>"$LOG_FILE" 2>&1 || true
import json
import os
from pathlib import Path

profile = Path(os.environ["PROFILE_DIR"])
pref = profile / "Default" / "Preferences"
pref.parent.mkdir(parents=True, exist_ok=True)

data = {}
if pref.exists():
    try:
        data = json.loads(pref.read_text(encoding="utf-8"))
    except Exception:
        data = {}

intl = data.setdefault("intl", {})
intl["accept_languages"] = "pt-BR,pt,en-US,en"
intl["selected_languages"] = "pt-BR,pt,en-US,en"

translate = data.setdefault("translate", {})
translate["enabled"] = False

prefs = data.setdefault("browser", {})
prefs["has_seen_welcome_page"] = True

pref.write_text(json.dumps(data, ensure_ascii=False, separators=(",", ":")), encoding="utf-8")
PY
}

detect_resolution() {
    local resolution=""
    if command -v xrandr >/dev/null 2>&1 && [[ -n "${DISPLAY:-}" ]]; then
        resolution="$(xrandr --current 2>/dev/null | awk '/\*/ {print $1; exit}')"
    fi

    if [[ "$resolution" =~ ^[0-9]+x[0-9]+$ ]]; then
        printf '%s\n' "$resolution"
    else
        printf '%s\n' "1920x1080"
    fi
}

launch_detached() {
    local unit
    unit="steam-media-${APP_ID}-$(date +%s)"
    sanitize_environment
    patch_chrome_preferences

    log "LD_PRELOAD após limpeza=${LD_PRELOAD:-}"
    log "DISPLAY preservado=${DISPLAY:-}"
    log "GAMESCOPE preservado=${GAMESCOPE_WAYLAND_DISPLAY:-}"
    log "Comando: $*"

    if command -v systemd-run >/dev/null 2>&1 && systemctl --user show-environment >/dev/null 2>&1; then
        log "Estratégia: systemd-run --user --scope"
        systemd-run --user --scope \
            --quiet \
            --unit="$unit" \
            --property=CollectMode=inactive-or-failed \
            -- "$@" >>"$LOG_FILE" 2>&1 &
        disown || true
        return 0
    fi

    if command -v setsid >/dev/null 2>&1; then
        log "Estratégia: setsid"
        setsid -f "$@" >>"$LOG_FILE" 2>&1
        return 0
    fi

    log "Estratégia: nohup"
    nohup "$@" >>"$LOG_FILE" 2>&1 </dev/null &
    disown || true
}

log "===== Inicialização ====="
log "Serviço: $APP_NAME"
log "URL: $APP_URL"
log "XDG_SESSION_TYPE=${XDG_SESSION_TYPE:-}"
log "DISPLAY=${DISPLAY:-}"
log "WAYLAND_DISPLAY=${WAYLAND_DISPLAY:-}"
log "GAMESCOPE_WAYLAND_DISPLAY=${GAMESCOPE_WAYLAND_DISPLAY:-}"
log "SteamGamepadUI=${SteamGamepadUI:-}"
log "LD_PRELOAD original=${LD_PRELOAD:-}"

MODE="auto"
if [[ -f "$PREF_FILE" ]]; then
    # shellcheck disable=SC1090
    source "$PREF_FILE"
fi

# v9: no Gaming Mode, kiosk é o padrão para tela cheia e sem barra.
if [[ "$MODE" == "auto" ]]; then
    if is_gaming_mode; then
        MODE="kiosk"
    else
        MODE="app"
    fi
fi

RESOLUTION="$(detect_resolution)"

COMMON_ARGS=(
    "--new-window"
    "--no-first-run"
    "--no-default-browser-check"
    "--disable-session-crashed-bubble"
    "--disable-features=Translate,TranslateUI"
    "--disable-translate"
    "--lang=pt-BR"
    "--password-store=basic"
    "--user-data-dir=$PROFILE_DIR"
    "--window-position=0,0"
    "--window-size=$RESOLUTION"
)

if is_gaming_mode; then
    COMMON_ARGS+=(
        "--ozone-platform=x11"
        "--start-fullscreen"
        "--start-maximized"
        "--hide-scrollbars"
    )
fi

if [[ "$MODE" == "xdg" ]]; then
    log "Modo selecionado: xdg-open"
    launch_detached /usr/bin/xdg-open "$APP_URL"
    exit 0
fi

if BROWSER="$(find_native_browser)"; then
    log "Navegador nativo: $BROWSER"
    case "$MODE" in
        kiosk)
            launch_detached "$BROWSER" "${COMMON_ARGS[@]}" "--kiosk" "$APP_URL"
            ;;
        browser)
            launch_detached "$BROWSER" "${COMMON_ARGS[@]}" "$APP_URL"
            ;;
        app|*)
            launch_detached "$BROWSER" "${COMMON_ARGS[@]}" "--app=$APP_URL"
            ;;
    esac
    exit 0
fi

if FLATPAK_APP="$(find_flatpak_browser)"; then
    log "Navegador Flatpak: $FLATPAK_APP"
    case "$MODE" in
        kiosk)
            launch_detached /usr/bin/flatpak run "$FLATPAK_APP" "${COMMON_ARGS[@]}" "--kiosk" "$APP_URL"
            ;;
        browser)
            launch_detached /usr/bin/flatpak run "$FLATPAK_APP" "${COMMON_ARGS[@]}" "$APP_URL"
            ;;
        app|*)
            launch_detached /usr/bin/flatpak run "$FLATPAK_APP" "${COMMON_ARGS[@]}" "--app=$APP_URL"
            ;;
    esac
    exit 0
fi

log "Nenhum navegador compatível encontrado; tentando xdg-open."
launch_detached /usr/bin/xdg-open "$APP_URL"
