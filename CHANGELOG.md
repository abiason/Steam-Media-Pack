# Histórico de alterações

Todas as mudanças relevantes do projeto são documentadas neste arquivo.

## [11.0] - 2026-07-22

### Adicionado

- Criação automática de atalhos não Steam para seis serviços.
- Leitor e gravador de `shortcuts.vdf` binário em Python.
- Backup automático de `shortcuts.vdf` antes da primeira alteração.
- Preservação dos atalhos não Steam existentes.
- Instalação automática das artes vertical, horizontal, hero, logo e ícone.
- Ferramenta para restaurar o backup dos atalhos.
- Ferramenta dedicada para atualizar somente as artes.

### Mantido

- Base funcional da versão 9.0.
- Modo `kiosk` em tela cheia no Gaming Mode.
- Modo `app` no Desktop Mode.
- Limpeza seletiva das variáveis de ambiente da Steam.
- Detecção de resolução com `xrandr`.
- Parâmetros de idioma `pt-BR` e tradução desativada.

## [10.0]

- Versão intermediária construída sobre a base funcional da 9.0.

## [9.0]

- Consolidação da base funcional utilizada pela versão 11.0.
- Correção do fechamento do navegador ao iniciar pela Steam.
