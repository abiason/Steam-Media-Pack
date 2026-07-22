# Configuração

## Arquivo de modo

O modo escolhido fica em:

```text
~/.config/steam-media-pack-11/mode.conf
```

Conteúdo padrão:

```bash
MODE=auto
```

## Alteração assistida

```bash
~/.local/share/steam-media-pack-11/selecionar-modo.sh
```

## Modos

### `auto`

- Desktop Mode: janela em modo aplicativo.
- Gaming Mode: navegador em modo `kiosk` e tela cheia.

### `kiosk`

Força o navegador sem barra de endereço e em tela cheia.

### `app`

Executa o serviço como uma janela de aplicativo do navegador. É a alternativa recomendada quando o modo `kiosk` apresenta incompatibilidade.

## Perfis do navegador

Cada launcher utiliza um perfil separado para reduzir conflitos entre os serviços e preservar sessões independentes.

## Logs

Os logs são gravados em:

```text
~/.local/state/steam-media-pack-11/logs/
```

Exemplo:

```text
~/.local/state/steam-media-pack-11/logs/netflix.log
```
