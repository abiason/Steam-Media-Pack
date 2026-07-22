# Solução de problemas

## A Steam informa que está aberta

Feche a interface e aguarde alguns segundos. Também pode executar:

```bash
steam -shutdown
```

Confirme:

```bash
pgrep -a steam
pgrep -a steamwebhelper
```

## O atalho não apareceu

- Confirme que o instalador terminou sem erros.
- Verifique se o usuário Steam correto foi selecionado.
- Reinicie completamente a Steam.
- Execute `./diagnostico.sh`.

## As artes não apareceram

Com a Steam fechada:

```bash
./tools/atualizar-artes-steam.sh
```

Depois reabra a Steam. Algumas telas podem manter cache até a reinicialização do cliente.

## O navegador abre e fecha no Gaming Mode

- Confirme que está usando a versão 11.0.
- Execute o seletor de modo e teste `app`:

```bash
~/.local/share/steam-media-pack-11/selecionar-modo.sh
```

- Consulte o log do serviço em `~/.local/state/steam-media-pack-11/logs/`.

## O serviço abre no Desktop Mode, mas não pela Steam

- Deixe a compatibilidade Proton desmarcada.
- Use `/usr/bin/bash` como destino.
- Confira se o caminho do launcher é absoluto.
- Não adicione parâmetros de Proton ou Wine.

## Não há acentos ou o teclado virtual não aparece

O teclado virtual é fornecido pela Steam ou pelo ambiente gráfico, não diretamente pelo pacote. No Gaming Mode, teste o atalho configurado pelo Steam Input para abrir o teclado.

## `xrandr` não está disponível

Instale o utilitário correspondente à sua distribuição. Em ambientes Wayland puros, a execução pode depender do XWayland.

## O login não é mantido

Verifique se o diretório de perfil do navegador tem permissão de escrita. Não execute os launchers como `root`.
