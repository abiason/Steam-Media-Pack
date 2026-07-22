# Instalação detalhada

## 1. Baixar o projeto

Pelo Git:

```bash
git clone URL_DO_REPOSITORIO.git
cd Steam-Media-Pack
```

Ou extraia o arquivo ZIP publicado na seção *Releases*.

## 2. Conceder permissões

```bash
chmod +x install.sh uninstall.sh diagnostico.sh selecionar-modo.sh
chmod +x launchers/*.sh tools/*.sh tools/*.py
```

## 3. Instalar os arquivos locais

```bash
./install.sh
```

O instalador cria:

```text
~/.local/share/steam-media-pack-11/
~/.config/steam-media-pack-11/
~/.local/share/applications/steam-media-*.desktop
~/.local/share/icons/hicolor/scalable/apps/steam-media-*.svg
```

## 4. Instalar os atalhos na Steam

Feche completamente a Steam:

```bash
steam -shutdown
```

Confirme que ela não está em execução e então execute:

```bash
./tools/instalar-atalhos-steam.sh
```

O programa procura usuários em:

```text
~/.local/share/Steam/userdata/
~/.steam/steam/userdata/
```

Quando houver mais de um usuário, será solicitado qual diretório deve ser alterado.

## 5. Abrir a Steam

Ao reabrir a Steam, os serviços devem aparecer na biblioteca como jogos não Steam.

## Instalação manual de um serviço

Exemplo para Netflix:

- Destino: `/usr/bin/bash`
- Iniciar em: `~/.local/share/steam-media-pack-11/launchers`
- Opções de inicialização: `"~/.local/share/steam-media-pack-11/launchers/netflix.sh"`
- Compatibilidade Proton: desmarcada

Use caminhos absolutos no campo da Steam; o caractere `~` é apenas uma abreviação usada na documentação.
