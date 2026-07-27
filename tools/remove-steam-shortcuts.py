#!/usr/bin/env python3

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path
from typing import Any

from lib.services import get_service_names
from lib.steam_users import choose_users, find_userdata, steam_running
from lib.steam_vdf import VDFError, load_vdf, save_vdf


SHORTCUTS_RELATIVE_PATH = Path("config") / "shortcuts.vdf"
GRID_RELATIVE_PATH = Path("config") / "grid"

BACKUP_FILENAME = (
    "shortcuts.vdf.before-steam-media-pack-removal"
)

STEAM_MEDIA_PACK_APPS = get_service_names()


def log(message: str) -> None:
    print(f"[INFO] {message}")


def warning(message: str) -> None:
    print(f"[AVISO] {message}")


def error(message: str) -> None:
    print(f"[ERRO] {message}", file=sys.stderr)


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


def shortcut_matches(
    shortcut: dict[str, Any],
) -> bool:
    app_name = str(shortcut.get("appname", "")).strip()

    return any(
        app_name.casefold() == service_name.casefold()
        for service_name in STEAM_MEDIA_PACK_APPS
    )


def normalize_appid(value: Any) -> int | None:
    try:
        return int(value) & 0xFFFFFFFF
    except (TypeError, ValueError):
        return None


def remove_shortcuts(
    shortcuts: dict[str, Any],
) -> tuple[dict[str, Any], list[tuple[str, int | None]]]:
    preserved: list[tuple[str, dict[str, Any]]] = []
    removed: list[tuple[str, int | None]] = []

    for original_index, shortcut in shortcuts.items():
        if not isinstance(shortcut, dict):
            preserved.append((str(original_index), shortcut))
            continue

        if shortcut_matches(shortcut):
            app_name = str(
                shortcut.get("appname", "Desconhecido")
            )
            appid = normalize_appid(shortcut.get("appid"))

            removed.append((app_name, appid))
            continue

        preserved.append((str(original_index), shortcut))

    rebuilt: dict[str, Any] = {}

    for new_index, (_, shortcut) in enumerate(preserved):
        rebuilt[str(new_index)] = shortcut

    return rebuilt, removed


def artwork_candidates(
    grid_directory: Path,
    appid: int,
) -> tuple[Path, ...]:
    prefixes = {
        str(appid),
        str(appid & 0xFFFFFFFF),
    }

    candidates: list[Path] = []

    for prefix in prefixes:
        candidates.extend(
            (
                grid_directory / f"{prefix}.png",
                grid_directory / f"{prefix}.jpg",
                grid_directory / f"{prefix}.jpeg",
                grid_directory / f"{prefix}.webp",
                grid_directory / f"{prefix}p.png",
                grid_directory / f"{prefix}p.jpg",
                grid_directory / f"{prefix}p.jpeg",
                grid_directory / f"{prefix}p.webp",
                grid_directory / f"{prefix}_hero.png",
                grid_directory / f"{prefix}_hero.jpg",
                grid_directory / f"{prefix}_hero.jpeg",
                grid_directory / f"{prefix}_hero.webp",
                grid_directory / f"{prefix}_logo.png",
                grid_directory / f"{prefix}_logo.jpg",
                grid_directory / f"{prefix}_logo.jpeg",
                grid_directory / f"{prefix}_logo.webp",
                grid_directory / f"{prefix}_icon.png",
                grid_directory / f"{prefix}_icon.jpg",
                grid_directory / f"{prefix}_icon.jpeg",
                grid_directory / f"{prefix}_icon.webp",
            )
        )

    return tuple(dict.fromkeys(candidates))


def remove_artwork(
    grid_directory: Path,
    appid: int | None,
) -> int:
    if appid is None or not grid_directory.is_dir():
        return 0

    removed = 0

    for artwork_path in artwork_candidates(
        grid_directory,
        appid,
    ):
        if not artwork_path.is_file():
            continue

        try:
            artwork_path.unlink()
        except OSError as exc:
            warning(
                f"Não foi possível remover a arte "
                f"{artwork_path}: {exc}"
            )
            continue

        removed += 1

    return removed


def create_backup(shortcuts_path: Path) -> Path:
    backup_path = shortcuts_path.with_name(BACKUP_FILENAME)

    try:
        shutil.copy2(shortcuts_path, backup_path)
    except OSError as exc:
        raise VDFError(
            f"Não foi possível criar o backup {backup_path}: {exc}"
        ) from exc

    return backup_path


def remove_for_user(user_directory: Path) -> tuple[int, int]:
    shortcuts_path = user_directory / SHORTCUTS_RELATIVE_PATH
    grid_directory = user_directory / GRID_RELATIVE_PATH

    if not shortcuts_path.is_file():
        warning(
            f"Arquivo shortcuts.vdf não encontrado para "
            f"o usuário {user_directory.name}."
        )
        return 0, 0

    document = load_vdf(shortcuts_path)
    shortcuts = get_shortcuts_container(document)

    rebuilt_shortcuts, removed_shortcuts = remove_shortcuts(
        shortcuts
    )

    if not removed_shortcuts:
        log(
            f"Nenhum atalho do Steam Media Pack encontrado "
            f"para {user_directory.name}."
        )
        return 0, 0

    backup_path = create_backup(shortcuts_path)
    log(f"Backup criado: {backup_path}")

    document["shortcuts"] = rebuilt_shortcuts
    save_vdf(shortcuts_path, document)

    artwork_count = 0

    for app_name, appid in removed_shortcuts:
        log(f"Atalho removido: {app_name}")

        removed_artworks = remove_artwork(
            grid_directory,
            appid,
        )

        artwork_count += removed_artworks

        if removed_artworks:
            log(
                f"{removed_artworks} arte(s) removida(s) "
                f"para {app_name}."
            )

    return len(removed_shortcuts), artwork_count


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Remove os atalhos e artes do Steam Media Pack "
            "dos perfis Steam."
        )
    )

    parser.add_argument(
        "--all-users",
        action="store_true",
        help="Remove de todos os usuários Steam sem perguntar.",
    )

    return parser.parse_args()


def main() -> int:
    args = parse_arguments()

    if steam_running():
        error(
            "A Steam está em execução. Feche completamente a Steam "
            "antes de remover os atalhos."
        )
        return 1

    users = find_userdata()

    if not users:
        warning("Nenhum usuário Steam foi encontrado.")
        return 0

    selected_users = users if args.all_users else choose_users(users)

    if not selected_users:
        warning("Nenhum usuário Steam foi selecionado.")
        return 0

    total_shortcuts = 0
    total_artworks = 0
    failures: list[str] = []

    for user_directory in selected_users:
        print()
        log(f"Processando usuário Steam: {user_directory.name}")

        try:
            shortcuts, artworks = remove_for_user(
                user_directory
            )
        except (OSError, VDFError, ValueError) as exc:
            failures.append(user_directory.name)
            error(
                f"Falha ao processar o usuário "
                f"{user_directory.name}: {exc}"
            )
            continue

        total_shortcuts += shortcuts
        total_artworks += artworks

    print()
    print("Resumo da remoção:")
    print(f"  Atalhos removidos: {total_shortcuts}")
    print(f"  Artes removidas: {total_artworks}")

    if failures:
        print(
            "  Usuários com falha: "
            + ", ".join(failures)
        )
        return 1

    print("  Remoção concluída com sucesso.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
