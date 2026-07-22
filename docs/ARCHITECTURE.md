# Arquitetura do projeto

## `launchers/`

Scripts responsáveis por:

- localizar um navegador compatível;
- detectar o modo de execução;
- preparar diretórios de perfil e log;
- preservar variáveis da sessão gráfica;
- remover seletivamente variáveis da Steam, Proton, Wine e Pressure Vessel que interferem no navegador;
- iniciar o site com parâmetros de tela cheia e idioma.

## `desktop/`

Arquivos `.desktop` instalados no menu de aplicativos do usuário.

## `icons/`

Ícones SVG usados pelos atalhos de desktop.

## `steam-art/`

Conjuntos de imagens para a biblioteca da Steam.

## `tools/instalar-atalhos-steam.py`

Implementa leitura e escrita do VDF binário, criação dos registros, backup e cópia das artes.

## `tools/atualizar-artes-steam.py`

Reutiliza o parser do instalador para localizar AppIDs já existentes e atualizar somente as imagens.

## `install.sh`

Copia launchers, ícones, atalhos de desktop e configuração padrão para os diretórios XDG do usuário.

## `uninstall.sh`

Remove os arquivos instalados pelo pacote sem editar automaticamente `shortcuts.vdf`.
