#!/usr/bin/env python3
from pathlib import Path
import sys

# Reuse the parser and artwork functions from the main installer.
sys.path.insert(0, str(Path(__file__).resolve().parent))
import importlib.util

module_path = Path(__file__).resolve().parent / "update-steam-arts.py"
spec = importlib.util.spec_from_file_location("smp11_installer", module_path)
mod = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)

def main():
    if mod.steam_running():
        print("Feche completamente a Steam antes de atualizar as artes.")
        raise SystemExit(2)

    users = mod.find_userdata()
    if not users:
        print("Nenhum usuário Steam encontrado.")
        raise SystemExit(3)

    _, userdir = mod.choose_user(users)
    config = userdir / "config"
    vdf = config / "shortcuts.vdf"

    if not vdf.exists():
        print("shortcuts.vdf não encontrado.")
        raise SystemExit(4)

    root = mod.parse_vdf(vdf.read_bytes())
    shortcuts = mod.get_shortcuts(root)
    existing = mod.existing_shortcuts(shortcuts)

    pack = Path(__file__).resolve().parent.parent
    art_root = pack / "steam-art"
    grid_dir = config / "grid"

    updated = []
    missing = []

    for sid, (name, _) in mod.SERVICES.items():
        item = existing.get(name)
        if not item:
            missing.append(name)
            continue
        mod.copy_art(grid_dir, item["appid"], art_root / sid)
        updated.append(name)

    print("Artes atualizadas:", ", ".join(updated) if updated else "nenhuma")
    if missing:
        print("Atalhos não encontrados:", ", ".join(missing))
    print("Pasta:", grid_dir)
    print("Abra novamente a Steam.")

if __name__ == "__main__":
    main()
