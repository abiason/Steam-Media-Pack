#!/usr/bin/env bash

set -euo pipefail

SERVICE_NAME="controller-bridge.service"
SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_FILE="${SOURCE_DIR}/${SERVICE_NAME}"
TARGET_DIR="${HOME}/.config/systemd/user"
TARGET_FILE="${TARGET_DIR}/${SERVICE_NAME}"

mkdir -p "${TARGET_DIR}"

install -m 0644 "${SOURCE_FILE}" "${TARGET_FILE}"

systemctl --user daemon-reload

echo "Serviço instalado em:"
echo "${TARGET_FILE}"
echo
echo "Para habilitar e iniciar:"
echo "systemctl --user enable --now ${SERVICE_NAME}"
