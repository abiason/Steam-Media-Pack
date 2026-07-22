# Como contribuir

Contribuições são bem-vindas por meio de *issues* e *pull requests*.

## Antes de começar

1. Pesquise se já existe uma *issue* sobre o assunto.
2. Descreva a distribuição, ambiente gráfico, versão da Steam e navegador.
3. Não publique credenciais, cookies, tokens, dados pessoais ou conteúdo protegido.

## Preparação do ambiente

```bash
git clone URL_DO_REPOSITORIO.git
cd Steam-Media-Pack
chmod +x *.sh launchers/*.sh tools/*.sh tools/*.py
```

## Validações mínimas

Execute antes de enviar um *pull request*:

```bash
bash -n install.sh uninstall.sh diagnostico.sh selecionar-modo.sh
bash -n launchers/*.sh tools/*.sh
python3 -m py_compile tools/*.py
```

## Diretrizes

- Preserve a compatibilidade com Bash.
- Use caminhos baseados em `$HOME`, `XDG_DATA_HOME` e `XDG_CONFIG_HOME` quando possível.
- Não remova atalhos do usuário sem confirmação explícita.
- Faça backup antes de alterar arquivos da Steam.
- Não introduza mecanismos para burlar DRM, bloqueios regionais ou autenticação.
- Documente mudanças relevantes no `CHANGELOG.md`.

## Relatos de erro

Inclua:

- distribuição e versão;
- ambiente gráfico;
- Desktop Mode ou Gaming Mode;
- navegador encontrado;
- serviço afetado;
- saída de `./diagnostico.sh`;
- trecho relevante do log, com informações privadas removidas.
