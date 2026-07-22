#!/usr/bin/env python3
from __future__ import annotations
from pathlib import Path
from collections import OrderedDict
import shutil, struct, zlib, re, subprocess, sys

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
    return data[pos:end].decode("utf-8", errors="replace"), end + 1

def parse_object(data: bytes, pos: int):
    obj = OrderedDict()
    while pos < len(data):
        t = data[pos]
        pos += 1
        if t == TYPE_END:
            return obj, pos
        key, pos = read_cstr(data, pos)
        if t == TYPE_OBJECT:
            val, pos = parse_object(data, pos)
        elif t == TYPE_STRING:
            val, pos = read_cstr(data, pos)
        elif t == TYPE_INT32:
            val = struct.unpack_from("<I", data, pos)[0]
            pos += 4
        elif t == TYPE_FLOAT32:
            val = struct.unpack_from("<f", data, pos)[0]
            pos += 4
        elif t == TYPE_PTR:
            val = struct.unpack_from("<I", data, pos)[0]
            pos += 4
        elif t == TYPE_WSTRING:
            chars = []
            while data[pos:pos+2] != b"\x00\x00":
                chars.append(data[pos:pos+2])
                pos += 2
            pos += 2
            val = b"".join(chars).decode("utf-16le", errors="replace")
        elif t == TYPE_COLOR:
            val = data[pos:pos+4]
            pos += 4
        elif t == TYPE_UINT64:
            val = struct.unpack_from("<Q", data, pos)[0]
            pos += 8
        else:
            raise ValueError(f"Tipo VDF binário não suportado: {t:#x}")
        obj[key] = (t, val)
    raise ValueError("VDF terminou sem marcador final")

def parse_vdf(data: bytes):
    if not data:
        return OrderedDict()
    root, pos = parse_object(data, 0)
    return root

def cstr(s: str) -> bytes:
    return s.encode("utf-8") + b"\x00"

def encode_entry(key: str, tv):
    t, val = tv
    out = bytes([t]) + cstr(key)
    if t == TYPE_OBJECT:
        out += encode_object(val)
    elif t == TYPE_STRING:
        out += cstr(str(val))
    elif t in (TYPE_INT32, TYPE_PTR):
        out += struct.pack("<I", int(val) & 0xffffffff)
    elif t == TYPE_FLOAT32:
        out += struct.pack("<f", float(val))
    elif t == TYPE_WSTRING:
        out += str(val).encode("utf-16le") + b"\x00\x00"
    elif t == TYPE_COLOR:
        raw = bytes(val)
        out += (raw + b"\x00\x00\x00\x00")[:4]
    elif t == TYPE_UINT64:
        out += struct.pack("<Q", int(val))
    else:
        raise ValueError(f"Tipo não suportado ao gravar: {t:#x}")
    return out

def encode_object(obj):
    out = bytearray()
    for key, tv in obj.items():
        out += encode_entry(key, tv)
    out.append(TYPE_END)
    return bytes(out)

def encode_vdf(root):
    return encode_object(root)

def steam_running():
    for name in ("steam", "steamwebhelper"):
        try:
            if subprocess.run(["pgrep","-x",name], capture_output=True).returncode == 0:
                return True
        except Exception:
            pass
    return False

def find_userdata():
    roots = [Path.home()/".local/share/Steam", Path.home()/".steam/steam"]
    found = []
    seen = set()
    for root in roots:
        try:
            real = root.resolve()
        except Exception:
            real = root
        if real in seen or not root.exists():
            continue
        seen.add(real)
        ud = root/"userdata"
        if not ud.exists():
            continue
        for p in ud.iterdir():
            if p.is_dir() and p.name.isdigit() and (p/"config").exists():
                found.append((root, p))
    return found

def make_shortcut(appname, script_path, launcher_dir):
    exe = "/usr/bin/bash"
    appid = (zlib.crc32((exe + appname).encode("utf-8")) | 0x80000000) & 0xffffffff
    tags = OrderedDict()
    return OrderedDict([
        ("appid", (TYPE_INT32, appid)),
        ("AppName", (TYPE_STRING, appname)),
        ("Exe", (TYPE_STRING, exe)),
        ("StartDir", (TYPE_STRING, str(launcher_dir))),
        ("icon", (TYPE_STRING, "")),
        ("ShortcutPath", (TYPE_STRING, "")),
        ("LaunchOptions", (TYPE_STRING, f'"{script_path}"')),
        ("IsHidden", (TYPE_INT32, 0)),
        ("AllowDesktopConfig", (TYPE_INT32, 1)),
        ("AllowOverlay", (TYPE_INT32, 0)),
        ("OpenVR", (TYPE_INT32, 0)),
        ("Devkit", (TYPE_INT32, 0)),
        ("DevkitGameID", (TYPE_STRING, "")),
        ("DevkitOverrideAppID", (TYPE_INT32, 0)),
        ("LastPlayTime", (TYPE_INT32, 0)),
        ("FlatpakAppID", (TYPE_STRING, "")),
        ("tags", (TYPE_OBJECT, tags)),
    ]), appid

def get_shortcuts(root):
    if "shortcuts" not in root:
        root["shortcuts"] = (TYPE_OBJECT, OrderedDict())
    t, shortcuts = root["shortcuts"]
    if t != TYPE_OBJECT:
        raise ValueError("Estrutura shortcuts inválida")
    return shortcuts

def existing_shortcuts(shortcuts):
    result = {}
    for _, (t, entry) in shortcuts.items():
        if t != TYPE_OBJECT:
            continue
        name_tv = entry.get("AppName") or entry.get("appname") or entry.get("appName")
        appid_tv = entry.get("appid")
        if name_tv and appid_tv:
            result[str(name_tv[1])] = {
                "entry": entry,
                "appid": int(appid_tv[1]) & 0xffffffff,
            }
    return result

def next_index(shortcuts):
    nums = []
    for key in shortcuts.keys():
        try:
            nums.append(int(key))
        except ValueError:
            pass
    return max(nums, default=-1) + 1

def copy_art(grid_dir, appid, service_dir):
    grid_dir.mkdir(parents=True, exist_ok=True)
    mapping = {
        "grid.png": f"{appid}p.png",
        "wide.png": f"{appid}.png",
        "hero.png": f"{appid}_hero.png",
        "logo.png": f"{appid}_logo.png",
        "icon.png": f"{appid}_icon.png",
    }
    for src, dst in mapping.items():
        p = service_dir/src
        if p.exists():
            shutil.copy2(p, grid_dir/dst)

def choose_user(users):
    if len(users) == 1:
        return users[0]
    print("Usuários Steam encontrados:")
    for i,(_,u) in enumerate(users,1):
        print(f"{i}) {u.name}")
    while True:
        try:
            n = int(input("Escolha: "))
            return users[n-1]
        except Exception:
            print("Escolha inválida.")

def main():
    if steam_running():
        print("Feche completamente a Steam antes de continuar.")
        sys.exit(2)

    users = find_userdata()
    if not users:
        print("Nenhum usuário Steam encontrado.")
        sys.exit(3)

    _, userdir = choose_user(users)
    config = userdir/"config"
    vdf = config/"shortcuts.vdf"
    backup = config/"shortcuts.vdf.steam-media-pack-11.backup"

    if vdf.exists():
        if not backup.exists():
            shutil.copy2(vdf, backup)
        root = parse_vdf(vdf.read_bytes())
    else:
        root = OrderedDict([("shortcuts", (TYPE_OBJECT, OrderedDict()))])

    shortcuts = get_shortcuts(root)
    existing = existing_shortcuts(shortcuts)
    index = next_index(shortcuts)

    pack = Path(__file__).resolve().parent.parent
    launcher_dir = Path.home()/".local/share/steam-media-pack-11/launchers"
    art_root = pack/"steam-art"
    grid_dir = config/"grid"

    added = []
    updated_art = []
    for sid,(name, script) in SERVICES.items():
        script_path = launcher_dir/script

        if name in existing:
            # O atalho já existe: preserva todos os seus campos e usa o
            # appid realmente armazenado pela Steam para nomear as artes.
            appid = existing[name]["appid"]
            copy_art(grid_dir, appid, art_root/sid)
            updated_art.append(name)
            continue

        entry, appid = make_shortcut(name, script_path, launcher_dir)
        shortcuts[str(index)] = (TYPE_OBJECT, entry)
        index += 1
        copy_art(grid_dir, appid, art_root/sid)
        added.append(name)
        updated_art.append(name)

    vdf.write_bytes(encode_vdf(root))

    print("Instalação concluída.")
    print("Usuário Steam:", userdir.name)
    print("Atalhos adicionados:", ", ".join(added) if added else "nenhum")
    print("Artes instaladas/atualizadas:", ", ".join(updated_art) if updated_art else "nenhuma")
    print("Backup:", backup if backup.exists() else "não havia arquivo anterior")
    print("Abra novamente a Steam.")

if __name__ == "__main__":
    main()
