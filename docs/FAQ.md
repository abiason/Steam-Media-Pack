# Perguntas frequentes

## Funciona somente no CachyOS?

A validação principal foi feita no CachyOS Handheld Edition. Outras distribuições podem funcionar, mas ainda não são oficialmente garantidas.

## Preciso ativar Proton?

Não. Os atalhos executam scripts Linux nativos e o navegador do sistema. Deixe a compatibilidade Proton desmarcada.

## O pacote fornece acesso gratuito aos serviços?

Não. Ele apenas cria atalhos para os sites oficiais. Assinaturas e contas continuam sendo necessárias quando exigidas.

## Ele contorna DRM?

Não. O projeto não remove nem contorna mecanismos de proteção.

## Posso trocar as artes?

Sim. Substitua os arquivos PNG da pasta correspondente e execute `tools/atualizar-artes-steam.sh` com a Steam fechada.

## Posso usar outro navegador?

Os launchers procuram navegadores Chromium compatíveis. Mudanças na ordem de detecção podem ser feitas diretamente nos scripts, mas devem ser testadas.

## A desinstalação remove os atalhos da Steam?

Não automaticamente. O script remove os arquivos locais do pacote. Para reverter o arquivo de atalhos ao estado inicial, utilize o backup antes de fazer outras alterações relevantes na biblioteca.

## Onde ficam os logs?

```text
~/.local/state/steam-media-pack-11/logs/
```
