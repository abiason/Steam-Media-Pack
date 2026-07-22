# Artes da biblioteca

Cada serviço possui uma pasta em `steam-art/` com os seguintes arquivos:

| Arquivo | Uso na Steam |
|---|---|
| `grid.png` | Capa vertical |
| `wide.png` | Grade horizontal |
| `hero.png` | Imagem de fundo da página do atalho |
| `logo.png` | Logotipo sobre o hero |
| `icon.png` | Ícone do atalho |

Também existem arquivos SVG de origem em algumas pastas.

## Nomes instalados

A ferramenta copia os arquivos para a pasta `config/grid` do usuário Steam usando:

```text
<appid>p.png
<appid>.png
<appid>_hero.png
<appid>_logo.png
<appid>_icon.png
```

## Atualização

Feche a Steam e execute:

```bash
./tools/atualizar-artes-steam.sh
```

A ferramenta lê os AppIDs dos atalhos existentes e substitui as imagens correspondentes, sem recriar os atalhos.

## Personalização

Substitua os PNGs na pasta do serviço mantendo os nomes esperados. Depois execute novamente a ferramenta de atualização.

## Direitos de terceiros

As marcas e logotipos pertencem aos respectivos titulares. Consulte `THIRD_PARTY_NOTICES.md` antes de redistribuir artes modificadas ou oficiais.
