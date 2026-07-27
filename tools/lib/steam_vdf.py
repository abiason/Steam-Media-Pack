from __future__ import annotations

import struct
from collections.abc import Mapping
from pathlib import Path
from typing import Any


TYPE_OBJECT = 0x00
TYPE_STRING = 0x01
TYPE_INT32 = 0x02
TYPE_END = 0x08


class VDFError(ValueError):
    """Erro ao interpretar um arquivo shortcuts.vdf."""


def read_cstring(data: bytes, offset: int) -> tuple[str, int]:
    end = data.find(b"\x00", offset)

    if end == -1:
        raise VDFError("String VDF sem terminador nulo.")

    try:
        value = data[offset:end].decode("utf-8")
    except UnicodeDecodeError:
        value = data[offset:end].decode("latin-1")

    return value, end + 1


def parse_object(data: bytes, offset: int = 0) -> tuple[dict[str, Any], int]:
    result: dict[str, Any] = {}

    while offset < len(data):
        value_type = data[offset]
        offset += 1

        if value_type == TYPE_END:
            return result, offset

        key, offset = read_cstring(data, offset)

        if value_type == TYPE_OBJECT:
            value, offset = parse_object(data, offset)

        elif value_type == TYPE_STRING:
            value, offset = read_cstring(data, offset)

        elif value_type == TYPE_INT32:
            if offset + 4 > len(data):
                raise VDFError("Inteiro VDF incompleto.")

            value = struct.unpack_from("<I", data, offset)[0]
            offset += 4

        else:
            raise VDFError(
                f"Tipo VDF não suportado: 0x{value_type:02x}"
            )

        result[key] = value

    raise VDFError("Objeto VDF sem marcador de encerramento.")


def parse_vdf(data: bytes) -> dict[str, Any]:
    result, offset = parse_object(data)

    if offset != len(data):
        remaining = data[offset:]

        if remaining.strip(b"\x00"):
            raise VDFError("Dados inesperados após o final do VDF.")

    return result


def encode_cstring(value: str) -> bytes:
    return value.encode("utf-8") + b"\x00"


def encode_object(data: Mapping[str, Any]) -> bytes:
    output = bytearray()

    for key, value in data.items():
        if isinstance(value, Mapping):
            output.append(TYPE_OBJECT)
            output.extend(encode_cstring(str(key)))
            output.extend(encode_object(value))

        elif isinstance(value, str):
            output.append(TYPE_STRING)
            output.extend(encode_cstring(str(key)))
            output.extend(encode_cstring(value))

        elif isinstance(value, int):
            output.append(TYPE_INT32)
            output.extend(encode_cstring(str(key)))
            output.extend(struct.pack("<I", value & 0xFFFFFFFF))

        else:
            raise VDFError(
                f"Valor não suportado para a chave {key!r}: "
                f"{type(value).__name__}"
            )

    output.append(TYPE_END)

    return bytes(output)


def encode_vdf(data: Mapping[str, Any]) -> bytes:
    return encode_object(data)


def load_vdf(path: Path) -> dict[str, Any]:
    try:
        return parse_vdf(path.read_bytes())
    except OSError as error:
        raise VDFError(
            f"Não foi possível ler {path}: {error}"
        ) from error


def save_vdf(path: Path, data: Mapping[str, Any]) -> None:
    temporary_path = path.with_suffix(path.suffix + ".tmp")

    try:
        temporary_path.write_bytes(encode_vdf(data))
        temporary_path.replace(path)
    except OSError as error:
        temporary_path.unlink(missing_ok=True)

        raise VDFError(
            f"Não foi possível salvar {path}: {error}"
        ) from error
