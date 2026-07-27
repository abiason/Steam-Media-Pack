#!/usr/bin/env bash

set -euo pipefail

SERVICE_NAME="controller-bridge.service"
TARGET_FILE="${HOME}/.config/systemd/user/${SERVICE_NAME}"

systemctl --user disable --now "${SERVICE_NAME}" 2>/dev/null || true

if [[ -f "${TARGET_FILE}" ]]
then
    rm -f "${TARGET_FILE}"
    echo "Serviço removido:"
    echo "${TARGET_FILE}"
else
    echo "Serviço não estava instalado."
fi

systemctl --user daemon-reload
systemctl --user reset-failed "${SERVICE_NAME}" 2>/dev/null || true
