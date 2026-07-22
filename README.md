# Steam Media Pack 11.0

Transforme a biblioteca da Steam em uma central de mídia no Linux, com atalhos para serviços de streaming executados pelo navegador em modo de tela cheia.

> Projeto comunitário e não oficial. Não possui vínculo com Valve, Netflix, Amazon, Disney, Warner Bros. Discovery, Google ou Spotify.

## Serviços incluídos

- Netflix
- Prime Video
- Disney+
- Max
- YouTube TV
- Spotify

## Principais recursos

- Launchers preparados para Desktop Mode e Gaming Mode.
- Modo `kiosk` em tela cheia no Gaming Mode.
- Limpeza seletiva das variáveis herdadas da Steam, sem remover variáveis essenciais da sessão gráfica.
- Detecção da resolução atual com `xrandr`.
- Idioma do navegador configurado como `pt-BR`.
- Criação automática dos atalhos não Steam.
- Instalação automática das artes da biblioteca.
- Preservação dos atalhos não Steam já existentes.
- Backup de `shortcuts.vdf` antes da primeira alteração.
- Ferramenta de restauração do backup.
- Seleção entre os modos `auto`, `kiosk` e `app`.
- Logs separados por serviço.

## Compatibilidade prevista

A versão 11.0 foi construída e validada no **CachyOS Handheld Edition**. Ela também pode funcionar em outras distribuições com Steam e navegador Chromium compatível, mas esses ambientes ainda não foram formalmente validados pelo projeto.

## Requisitos

- Linux com sessão gráfica X11 ou XWayland capaz de executar `xrandr`.
- Steam para Linux.
- Bash.
- Python 3 para a integração automática com a Steam.
- Google Chrome, Chromium ou navegador compatível detectado pelos launchers.
- Acesso às plataformas de streaming e assinatura ativa quando exigida pelo serviço.

## Instalação rápida

Baixe ou clone o projeto e execute:

```bash
chmod +x install.sh uninstall.sh diagnostico.sh selecionar-modo.sh
chmod +x launchers/*.sh tools/*.sh tools/*.py
./install.sh
```

O conteúdo principal será instalado em:

```text
~/.local/share/steam-media-pack-11
```

As configurações serão gravadas em:

```text
~/.config/steam-media-pack-11
```

### Instalar automaticamente os atalhos na Steam

1. Feche completamente a Steam, inclusive o processo em segundo plano.
2. Execute:

```bash
./tools/instalar-atalhos-steam.sh
```

3. Abra novamente a Steam.

O instalador adiciona somente os serviços ausentes e preserva os atalhos não Steam já cadastrados.

## Alterar o modo de abertura

```bash
~/.local/share/steam-media-pack-11/selecionar-modo.sh
```

Modos disponíveis:

- `auto`: usa `app` no Desktop Mode e `kiosk` no Gaming Mode.
- `kiosk`: força tela cheia sem interface do navegador.
- `app`: abre como aplicativo do navegador.

## Atualizar somente as artes

Feche completamente a Steam e execute:

```bash
./tools/atualizar-artes-steam.sh
```

## Restaurar o backup dos atalhos

Feche completamente a Steam e execute:

```bash
./tools/restaurar-atalhos-steam.sh
```

## Diagnóstico

```bash
./diagnostico.sh
```

## Desinstalação

```bash
./uninstall.sh
```

A desinstalação remove os launchers, arquivos `.desktop`, ícones e configurações do pacote. Ela não remove automaticamente os atalhos já gravados em `shortcuts.vdf`.

## Documentação

- [Instalação detalhada](docs/INSTALLATION.md)
- [Configuração e modos](docs/CONFIGURATION.md)
- [Serviços incluídos](docs/SERVICES.md)
- [Integração com a Steam](docs/STEAM_INTEGRATION.md)
- [Artes da biblioteca](docs/ARTWORK.md)
- [Solução de problemas](docs/TROUBLESHOOTING.md)
- [Perguntas frequentes](docs/FAQ.md)
- [Arquitetura do projeto](docs/ARCHITECTURE.md)

## Estrutura do projeto

```text
Steam-Media-Pack-11.0/
├── .github/
├── desktop/
├── docs/
├── icons/
├── launchers/
├── steam-art/
├── tools/
├── CHANGELOG.md
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── LICENSE
├── README.md
├── ROADMAP.md
├── SECURITY.md
├── diagnostico.sh
├── install.sh
├── selecionar-modo.sh
└── uninstall.sh
```

## Avisos importantes

Os serviços de streaming podem alterar seus sites, mecanismos de autenticação, DRM e requisitos de navegador sem aviso. O projeto não contorna DRM, não fornece contas e não distribui conteúdo protegido.

As marcas, nomes e logotipos pertencem aos respectivos titulares. Consulte [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Licença

O código do projeto é disponibilizado sob a [Licença MIT](LICENSE). Marcas e elementos de terceiros não são licenciados pelo projeto.
