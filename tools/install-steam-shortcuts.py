#!/usr/bin/env python3
from __future__ import annotations

from collections import OrderedDict
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import zlib


SERVICES = {
    "netflix": ("Netflix", "netflix.sh"),
    "prime-video": ("Prime Video", "prime-video.sh"),
    "disney-plus": ("Disney+", "disney-plus.sh"),
    "max": ("Max", "max.sh"),
    "youtube-tv": ("YouTube TV", "youtube-tv.sh"),
    "spotify": ("Spotify", "spotify.sh"),
}

TYPE_OBJECT = 0x00
TYPE_STRING = 0x01
TYPE_INT32 = 0x02
TYPE_FLOAT32 = 0x03
TYPE_PTR = 0x04
TYPE_WSTRING = 0x05
TYPE_COLOR = 0x06
TYPE_UINT64 = 0x07
TYPE_END = 0x08


def read_cstr(data: bytes, pos: int):
    end = data.index(b"\x00", pos)

    return (
        data[pos:end].decode("utf-8", errors="replace"),
        end + 1,
    )


def parse_object(data: bytes, pos: int):
    obj = OrderedDict()

    while pos < len(data):
        value_type = data[pos]
        pos += 1

        if value_type == TYPE_END:
            return obj, pos

        key, pos = read_cstr(data, pos)

        if value_type == TYPE_OBJECT:
            value, pos = parse_object(data, pos)

        elif value_type == TYPE_STRING:
            value, pos = read_cstr(data, pos)

        elif value_type == TYPE_INT32:
            value = struct.unpack_from("<I", data, pos)[0]
            pos += 4

        elif value_type == TYPE_FLOAT32:
            value = struct.unpack_from("<f", data, pos)[0]
            pos += 4

        elif value_type == TYPE_PTR:
            value = struct.unpack_from("<I", data, pos)[0]
            pos += 4

        elif value_type == TYPE_WSTRING:
            chars = []

            while data[pos : pos + 2] != b"\x00\x00":
                chars.append(data[pos : pos + 2])
                pos += 2

            pos += 2

            value = b"".join(chars).decode(
                "utf-16le",
                errors="replace",
            )

        elif value_type == TYPE_COLOR:
            value = data[pos : pos + 4]
            pos += 4

        elif value_type == TYPE_UINT64:
            value = struct.unpack_from("<Q", data, pos)[0]
            pos += 8

        else:
            raise ValueError(
                "Tipo VDF binário não suportado: "
                f"{value_type:#x}"
            )

        obj[key] = (value_type, value)

    raise ValueError("VDF terminou sem marcador final")


def parse_vdf(data: bytes):
    if not data:
        return OrderedDict()

    root, _ = parse_object(data, 0)

    return root


def cstr(value: str) -> bytes:
    return value.encode("utf-8") + b"\x00"


def encode_entry(key: str, typed_value):
    value_type, value = typed_value

    output = bytes([value_type]) + cstr(key)

    if value_type == TYPE_OBJECT:
        output += encode_object(value)

    elif value_type == TYPE_STRING:
        output += cstr(str(value))

    elif value_type in (TYPE_INT32, TYPE_PTR):
        output += struct.pack(
            "<I",
            int(value) & 0xFFFFFFFF,
        )

    elif value_type == TYPE_FLOAT32:
        output += struct.pack("<f", float(value))

    elif value_type == TYPE_WSTRING:
        output += (
            str(value).encode("utf-16le")
            + b"\x00\x00"
        )

    elif value_type == TYPE_COLOR:
        raw_value = bytes(value)
        output += (
            raw_value + b"\x00\x00\x00\x00"
        )[:4]

    elif value_type == TYPE_UINT64:
        output += struct.pack("<Q", int(value))

    else:
        raise ValueError(
            "Tipo não suportado ao gravar: "
            f"{value_type:#x}"
        )

    return output


def encode_object(obj):
    output = bytearray()

    for key, typed_value in obj.items():
        output += encode_entry(key, typed_value)

    output.append(TYPE_END)

    return bytes(output)


def encode_vdf(root):
    return encode_object(root)


def steam_running():
    for process_name in ("steam", "steamwebhelper"):
        try:
            result = subprocess.run(
                ["pgrep", "-x", process_name],
                capture_output=True,
                check=False,
            )

            if result.returncode == 0:
                return True

        except OSError:
            pass

    return False


def find_userdata():
    steam_roots = [
        Path.home() / ".local/share/Steam",
        Path.home() / ".steam/steam",
    ]

    users = []
    seen_roots = set()
    seen_users = set()

    for steam_root in steam_roots:
        try:
            real_root = steam_root.resolve()
        except OSError:
            real_root = steam_root

        if real_root in seen_roots:
            continue

        if not steam_root.exists():
            continue

        seen_roots.add(real_root)

        userdata_dir = steam_root / "userdata"

        if not userdata_dir.exists():
            continue

        for user_dir in sorted(
            userdata_dir.iterdir(),
            key=lambda path: path.name,
        ):
            if not user_dir.is_dir():
                continue

            if not user_dir.name.isdigit():
                continue

            if not (user_dir / "config").exists():
                continue

            try:
                real_user_dir = user_dir.resolve()
            except OSError:
                real_user_dir = user_dir

            if real_user_dir in seen_users:
                continue

            seen_users.add(real_user_dir)
            users.append((steam_root, user_dir))

    return users


def make_shortcut(
    app_name: str,
    script_path: Path,
    launcher_dir: Path,
):
    executable = "/usr/bin/bash"

    app_id = (
        zlib.crc32(
            (executable + app_name).encode("utf-8")
        )
        | 0x80000000
    ) & 0xFFFFFFFF

    tags = OrderedDict()

    entry = OrderedDict(
        [
            ("appid", (TYPE_INT32, app_id)),
            ("AppName", (TYPE_STRING, app_name)),
            ("Exe", (TYPE_STRING, executable)),
            (
                "StartDir",
                (TYPE_STRING, str(launcher_dir)),
            ),
            ("icon", (TYPE_STRING, "")),
            ("ShortcutPath", (TYPE_STRING, "")),
            (
                "LaunchOptions",
                (
                    TYPE_STRING,
                    f'"{script_path}"',
                ),
            ),
            ("IsHidden", (TYPE_INT32, 0)),
            (
                "AllowDesktopConfig",
                (TYPE_INT32, 1),
            ),
            ("AllowOverlay", (TYPE_INT32, 0)),
            ("OpenVR", (TYPE_INT32, 0)),
            ("Devkit", (TYPE_INT32, 0)),
            ("DevkitGameID", (TYPE_STRING, "")),
            (
                "DevkitOverrideAppID",
                (TYPE_INT32, 0),
            ),
            ("LastPlayTime", (TYPE_INT32, 0)),
            ("FlatpakAppID", (TYPE_STRING, "")),
            ("tags", (TYPE_OBJECT, tags)),
        ]
    )

    return entry, app_id


def get_shortcuts(root):
    if "shortcuts" not in root:
        root["shortcuts"] = (
            TYPE_OBJECT,
            OrderedDict(),
        )

    value_type, shortcuts = root["shortcuts"]

    if value_type != TYPE_OBJECT:
        raise ValueError(
            "Estrutura shortcuts inválida"
        )

    return shortcuts


def existing_shortcuts(shortcuts):
    result = {}

    for _, typed_entry in shortcuts.items():
        value_type, entry = typed_entry

        if value_type != TYPE_OBJECT:
            continue

        name_value = (
            entry.get("AppName")
            or entry.get("appname")
            or entry.get("appName")
        )

        app_id_value = entry.get("appid")

        if not name_value or not app_id_value:
            continue

        result[str(name_value[1])] = {
            "entry": entry,
            "appid": (
                int(app_id_value[1])
                & 0xFFFFFFFF
            ),
        }

    return result


def next_index(shortcuts):
    indexes = []

    for key in shortcuts:
        try:
            indexes.append(int(key))
        except ValueError:
            continue

    return max(indexes, default=-1) + 1


def copy_art(
    grid_dir: Path,
    app_id: int,
    service_dir: Path,
):
    grid_dir.mkdir(
        parents=True,
        exist_ok=True,
    )

    art_mapping = {
        "grid.png": f"{app_id}p.png",
        "wide.png": f"{app_id}.png",
        "hero.png": f"{app_id}_hero.png",
        "logo.png": f"{app_id}_logo.png",
        "icon.png": f"{app_id}_icon.png",
    }

    for source_name, destination_name in (
        art_mapping.items()
    ):
        source_file = service_dir / source_name
        destination_file = (
            grid_dir / destination_name
        )

        if source_file.exists():
            shutil.copy2(
                source_file,
                destination_file,
            )


def choose_users(users):
    if len(users) == 1:
        return users

    print()
    print("Usuários Steam encontrados:")
    print("0) Todos os usuários")

    for index, (_, user_dir) in enumerate(
        users,
        start=1,
    ):
        print(f"{index}) {user_dir.name}")

    while True:
        try:
            choice = int(
                input("Escolha: ").strip()
            )
        except ValueError:
            print("Escolha inválida.")
            continue

        if choice == 0:
            return users

        if 1 <= choice <= len(users):
            return [users[choice - 1]]

        print("Escolha inválida.")


def install_for_user(
    user_dir: Path,
    launcher_dir: Path,
    art_root: Path,
):
    config_dir = user_dir / "config"

    shortcuts_file = (
        config_dir / "shortcuts.vdf"
    )

    backup_file = (
        config_dir
        / "shortcuts.vdf.steam-media-pack.backup"
    )

    config_dir.mkdir(
        parents=True,
        exist_ok=True,
    )

    if shortcuts_file.exists():
        if not backup_file.exists():
            shutil.copy2(
                shortcuts_file,
                backup_file,
            )

        root = parse_vdf(
            shortcuts_file.read_bytes()
        )

    else:
        root = OrderedDict(
            [
                (
                    "shortcuts",
                    (
                        TYPE_OBJECT,
                        OrderedDict(),
                    ),
                )
            ]
        )

    shortcuts = get_shortcuts(root)
    existing = existing_shortcuts(shortcuts)
    shortcut_index = next_index(shortcuts)

    grid_dir = config_dir / "grid"

    added = []
    updated_art = []

    for service_id, service_data in (
        SERVICES.items()
    ):
        app_name, script_name = service_data

        script_path = (
            launcher_dir / script_name
        )

        if not script_path.exists():
            print(
                "[AVISO] Launcher não encontrado: "
                f"{script_path}"
            )
            continue

        if app_name in existing:
            app_id = existing[app_name]["appid"]

            copy_art(
                grid_dir,
                app_id,
                art_root / service_id,
            )

            updated_art.append(app_name)
            continue

        shortcut_entry, app_id = make_shortcut(
            app_name,
            script_path,
            launcher_dir,
        )

        shortcuts[str(shortcut_index)] = (
            TYPE_OBJECT,
            shortcut_entry,
        )

        shortcut_index += 1

        copy_art(
            grid_dir,
            app_id,
            art_root / service_id,
        )

        added.append(app_name)
        updated_art.append(app_name)

    shortcuts_file.write_bytes(
        encode_vdf(root)
    )

    print()
    print(
        "Usuário Steam:",
        user_dir.name,
    )

    print(
        "Atalhos adicionados:",
        ", ".join(added)
        if added
        else "nenhum",
    )

    print(
        "Artes instaladas/atualizadas:",
        ", ".join(updated_art)
        if updated_art
        else "nenhuma",
    )

    print(
        "Backup:",
        backup_file
        if backup_file.exists()
        else "não havia arquivo anterior",
    )


def main():
    if steam_running():
        print(
            "Feche completamente a Steam "
            "antes de continuar."
        )
        sys.exit(2)

    users = find_userdata()

    if not users:
        print("Nenhum usuário Steam encontrado.")
        sys.exit(3)

    selected_users = choose_users(users)

    project_dir = (
        Path(__file__).resolve().parent.parent
    )

    launcher_dir = (
        Path.home()
        / ".local/share/steam-media-pack/launchers"
    )

    art_root = project_dir / "steam-art"

    if not launcher_dir.exists():
        print(
            "Diretório de launchers não encontrado:"
        )
        print(launcher_dir)
        sys.exit(4)

    failures = []

    for _, user_dir in selected_users:
        try:
            install_for_user(
                user_dir,
                launcher_dir,
                art_root,
            )

        except Exception as error:
            failures.append(
                (user_dir.name, str(error))
            )

            print()
            print(
                "[ERRO] Falha ao instalar para "
                f"o usuário {user_dir.name}:"
            )
            print(error)

    print()

    if failures:
        print(
            "A instalação terminou com erros "
            "em alguns usuários."
        )

        for user_id, error in failures:
            print(f"- {user_id}: {error}")

        sys.exit(5)

    print(
        "Instalação concluída para "
        f"{len(selected_users)} usuário(s) Steam."
    )

    print("Abra novamente a Steam.")


if __name__ == "__main__":
    main()
