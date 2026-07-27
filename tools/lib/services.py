from __future__ import annotations


SERVICES = {
    "netflix": {
        "name": "Netflix",
        "launcher": "netflix.sh",
    },
    "prime-video": {
        "name": "Prime Video",
        "launcher": "prime-video.sh",
    },
    "disney-plus": {
        "name": "Disney+",
        "launcher": "disney-plus.sh",
    },
    "max": {
        "name": "Max",
        "launcher": "max.sh",
    },
    "youtube-tv": {
        "name": "YouTube TV",
        "launcher": "youtube-tv.sh",
    },
    "spotify": {
        "name": "Spotify",
        "launcher": "spotify.sh",
    },
}


def get_service_names() -> set[str]:
    """Retorna os nomes exibidos na biblioteca Steam."""

    return {
        service["name"]
        for service in SERVICES.values()
    }
