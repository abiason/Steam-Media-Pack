#!/usr/bin/env python3

from __future__ import annotations

import argparse
import binascii
import shutil
import sys
from pathlib import Path
from typing import Any

from lib.services import SERVICES
from lib.steam_users import choose_users, find_userdata, steam_running
from lib.steam_vdf import VDFError, load_vdf, save_vdf


PROJECT_DIR = Path(__file__).resolve().parent.parent

INSTALLED_LAUNCHERS_DIR = (
    Path.home()
    / ".local"
    / "share"
    / "steam-media-pack"
    / "launchers"
)

STEAM_ART_DIR = PROJECT_DIR / "steam-art"

SHORTCUTS_RELATIVE_PATH = Path("config") / "shortcuts.vdf"
GRID_RELATIVE_PATH = Path("config") / "grid"

BACKUP_FILENAME = "shortcuts.vdf.before-steam-media-pack-install"


def log(message: str) -> None:
    print(f"[INFO] {message}")


def warning(message: str) -> None:
    print(f"[AVISO] {message}")


def error(message: str) -> None:
    print(f"[ERRO] {message}", file=sys.stderr)


def calculate_appid(executable: str, app_name: str) -> int:
    """
    Calcula o AppID de um atalho não Steam.

    A Steam utiliza o CRC32 da concatenação entre o executável e o nome
    do aplicativo. O bit mais significativo deve estar habilitado.
    """

    source = f"{executable}{app_name}".encode("utf-8")
    checksum = binascii.crc32(source) & 0xFFFFFFFF

    return checksum | 0x80000000


def normalize_executable(path: Path) -> str:
    return f'"{path}"'


def normalize_start_directory(path: Path) -> str:
    return f'"{path}"'


def get_shortcuts_container(
    document: dict[str, Any],
) -> dict[str, Any]:
    shortcuts = document.get("shortcuts")

    if shortcuts is None:
        shortcuts = {}
        document["shortcuts"] = shortcuts

    if not isinstance(shortcuts, dict):
        raise VDFError(
            "A chave 'shortcuts' não contém um objeto VDF válido."
        )

    return shortcuts


def next_shortcut_index(shortcuts: dict[str, Any]) -> str:
    numeric_indexes = []

    for key in shortcuts:
        if str(key).isdigit():
            numeric_indexes.append(int(key))

    if not numeric_indexes:
        return "0"

    return str(max(numeric_indexes) + 1)


def find_existing_shortcut(
    shortcuts: dict[str, Any],
    app_name: str,
) -> tuple[str, dict[str, Any]] | None:
    for index, shortcut in shortcuts.items():
        if not isinstance(shortcut, dict):
            continue

        current_name = str(shortcut.get("appname", "")).strip()

        if current_name.casefold() == app_name.casefold():
            return str(index), shortcut

    return None


def build_shortcut(
    app_name: str,
    launcher_path: Path,
) -> dict[str, Any]:
    executable = normalize_executable(launcher_path)
    start_directory = normalize_start_directory(launcher_path.parent)
    appid = calculate_appid(executable, app_name)

    return {
        "appid": appid,
        "appname": app_name,
        "exe": executable,
        "StartDir": start_directory,
        "icon": "",
        "ShortcutPath": "",
        "LaunchOptions": "",
        "IsHidden": 0,
        "AllowDesktopConfig": 1,
        "AllowOverlay": 1,
        "OpenVR": 0,
        "Devkit": 0,
        "DevkitGameID": "",
        "DevkitOverrideAppID": 0,
        "LastPlayTime": 0,
        "FlatpakAppID": "",
        "tags": {
            "0": "Steam Media Pack",
        },
    }


def update_shortcut(
    shortcut: dict[str, Any],
    app_name: str,
    launcher_path: Path,
) -> int:
    executable = normalize_executable(launcher_path)
    start_directory = normalize_start_directory(launcher_path.parent)
    appid = calculate_appid(executable, app_name)

    shortcut["appid"] = appid
    shortcut["appname"] = app_name
    shortcut["exe"] = executable
    shortcut["StartDir"] = start_directory
    shortcut.setdefault("icon", "")
    shortcut.setdefault("ShortcutPath", "")
    shortcut.setdefault("LaunchOptions", "")
    shortcut.setdefault("IsHidden", 0)
    shortcut.setdefault("AllowDesktopConfig", 1)
    shortcut.setdefault("AllowOverlay", 1)
    shortcut.setdefault("OpenVR", 0)
    shortcut.setdefault("Devkit", 0)
    shortcut.setdefault("DevkitGameID", "")
    shortcut.setdefault("DevkitOverrideAppID", 0)
    shortcut.setdefault("LastPlayTime", 0)
    shortcut.setdefault("FlatpakAppID", "")

    tags = shortcut.get("tags")

    if not isinstance(tags, dict):
        tags = {}

    if "Steam Media Pack" not in tags.values():
        index = 0

        while str(index) in tags:
            index += 1

        tags[str(index)] = "Steam Media Pack"

    shortcut["tags"] = tags

    return appid


def copy_if_exists(source: Path, destination: Path) -> bool:
    if not source.is_file():
        return False

    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)

    return True


def install_artwork(
    grid_directory: Path,
    service_id: str,
    appid: int,
) -> int:
    """
    Instala artes usando os nomes mais comuns do projeto.

    Arquivos aceitos dentro de steam-art/<serviço>/:

    - grid.png ou portrait.png
    - hero.png
    - logo.png
    - icon.png
    - grid-wide.png ou capsule.png
    """

    source_directory = STEAM_ART_DIR / service_id

    if not source_directory.is_dir():
        warning(
            f"Diretório de artes não encontrado para "
            f"{service_id}: {source_directory}"
        )
        return 0

    mappings = (
        (
            ("grid.png", "portrait.png"),
            grid_directory / f"{appid}p.png",
        ),
        (
            ("hero.png",),
            grid_directory / f"{appid}_hero.png",
        ),
        (
            ("logo.png",),
            grid_directory / f"{appid}_logo.png",
        ),
        (
            ("grid-wide.png", "capsule.png", "wide.png"),
            grid_directory / f"{appid}.png",
        ),
        (
            ("icon.png",),
            grid_directory / f"{appid}_icon.png",
        ),
    )

    installed = 0

    for source_names, destination in mappings:
        for source_name in source_names:
            source = source_directory / source_name

            if copy_if_exists(source, destination):
                installed += 1
                break

    return installed


def create_backup(shortcuts_path: Path) -> Path | None:
    if not shortcuts_path.is_file():
        return None

    backup_path = shortcuts_path.with_name(BACKUP_FILENAME)

    try:
        shutil.copy2(shortcuts_path, backup_path)
    except OSError as exc:
        raise VDFError(
            f"Não foi possível criar o backup {backup_path}: {exc}"
        ) from exc

    return backup_path


def load_shortcuts_document(
    shortcuts_path: Path,
) -> dict[str, Any]:
    if not shortcuts_path.exists():
        return {"shortcuts": {}}

    return load_vdf(shortcuts_path)


def install_for_user(user_directory: Path) -> tuple[int, int, int]:
    shortcuts_path = user_directory / SHORTCUTS_RELATIVE_PATH
    grid_directory = user_directory / GRID_RELATIVE_PATH

    shortcuts_path.parent.mkdir(parents=True, exist_ok=True)
    grid_directory.mkdir(parents=True, exist_ok=True)

    document = load_shortcuts_document(shortcuts_path)
    shortcuts = get_shortcuts_container(document)

    backup_path = create_backup(shortcuts_path)

    if backup_path is not None:
        log(f"Backup criado: {backup_path}")

    created = 0
    updated = 0
    artworks = 0

    for service_id, service in SERVICES.items():
        app_name = service["name"]
        launcher_filename = service["launcher"]
        launcher_path = INSTALLED_LAUNCHERS_DIR / launcher_filename

        if not launcher_path.is_file():
            warning(
                f"Launcher não encontrado para {app_name}: "
                f"{launcher_path}"
            )
            continue

        existing = find_existing_shortcut(shortcuts, app_name)

        if existing is None:
            shortcut_index = next_shortcut_index(shortcuts)
            shortcut = build_shortcut(app_name, launcher_path)
            shortcuts[shortcut_index] = shortcut

            appid = int(shortcut["appid"])
            created += 1

            log(f"Atalho criado: {app_name}")
        else:
            _, shortcut = existing
            appid = update_shortcut(
                shortcut,
                app_name,
                launcher_path,
            )

            updated += 1
            log(f"Atalho atualizado: {app_name}")

        installed_artworks = install_artwork(
            grid_directory,
            service_id,
            appid,
        )

        artworks += installed_artworks

        if installed_artworks:
            log(
                f"{installed_artworks} arte(s) instalada(s) "
                f"para {app_name}."
            )

    save_vdf(shortcuts_path, document)

    return created, updated, artworks


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Instala os atalhos do Steam Media Pack "
            "nos perfis Steam."
        )
    )

    parser.add_argument(
        "--all-users",
        action="store_true",
        help="Instala para todos os usuários Steam sem perguntar.",
    )

    return parser.parse_args()


def main() -> int:
    args = parse_arguments()

    if steam_running():
        error(
            "A Steam está em execução. Feche completamente a Steam "
            "antes de instalar os atalhos."
        )
        return 1

    users = find_userdata()

    if not users:
        error("Nenhum usuário Steam foi encontrado.")
        return 1

    selected_users = users if args.all_users else choose_users(users)

    if not selected_users:
        error("Nenhum usuário Steam foi selecionado.")
        return 1

    total_created = 0
    total_updated = 0
    total_artworks = 0
    failures: list[str] = []

    for user_directory in selected_users:
        print()
        log(f"Processando usuário Steam: {user_directory.name}")

        try:
            created, updated, artworks = install_for_user(
                user_directory
            )
        except (OSError, VDFError, ValueError) as exc:
            failures.append(user_directory.name)
            error(
                f"Falha ao processar o usuário "
                f"{user_directory.name}: {exc}"
            )
            continue

        total_created += created
        total_updated += updated
        total_artworks += artworks

    print()
    print("Resumo da instalação:")
    print(f"  Atalhos criados: {total_created}")
    print(f"  Atalhos atualizados: {total_updated}")
    print(f"  Artes instaladas: {total_artworks}")

    if failures:
        print(
            "  Usuários com falha: "
            + ", ".join(failures)
        )
        return 1

    print("  Instalação concluída com sucesso.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
