from __future__ import annotations

from pathlib import Path
import subprocess


STEAM_USERDATA_DIRS = (
    Path.home() / ".local/share/Steam/userdata",
    Path.home() / ".steam/steam/userdata",
)


def steam_running() -> bool:
    return subprocess.run(
        ["pgrep", "-x", "steam"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    ).returncode == 0


def find_userdata() -> list[Path]:
    found = []

    seen = set()

    for base in STEAM_USERDATA_DIRS:

        if not base.exists():
            continue

        for user in base.iterdir():

            if not user.is_dir():
                continue

            real = user.resolve()

            if real in seen:
                continue

            seen.add(real)
            found.append(real)

    return sorted(found)


def choose_users(users: list[Path]) -> list[Path]:

    if not users:
        return []

    if len(users) == 1:
        return users

    print()

    print("Usuários Steam encontrados:")

    print("0) Todos")

    for index, user in enumerate(users, start=1):
        print(f"{index}) {user.name}")

    print()

    while True:

        option = input("Escolha: ").strip()

        if option == "0":
            return users

        if option.isdigit():

            value = int(option)

            if 1 <= value <= len(users):
                return [users[value - 1]]

        print("Opção inválida.")
