# Integração com a Steam

## Arquivo `shortcuts.vdf`

A Steam armazena os atalhos não Steam em um arquivo VDF binário por usuário:

```text
<Steam>/userdata/<ID>/config/shortcuts.vdf
```

O instalador em Python:

1. verifica se a Steam está fechada;
2. localiza os diretórios de usuário;
3. cria um backup quando existe arquivo anterior;
4. lê a estrutura binária;
5. preserva os registros existentes;
6. adiciona apenas os serviços ausentes;
7. grava as artes usando o `appid` correspondente.

## Backup

O backup padrão é:

```text
shortcuts.vdf.steam-media-pack-11.backup
```

Ele é criado somente quando ainda não existe, evitando substituir o backup inicial por alterações posteriores.

## Restauração

```bash
./tools/restaurar-atalhos-steam.sh
```

A Steam deve permanecer fechada durante a restauração.

## AppID dos atalhos

Para novos atalhos, o projeto calcula o identificador com CRC32 sobre o executável e o nome do aplicativo, marcando o bit reservado aos atalhos não Steam. Para atalhos já existentes, a ferramenta utiliza o `appid` realmente armazenado no VDF.

## Cuidados

Embora o instalador preserve os registros lidos, alterações em arquivos internos da Steam sempre devem ser feitas com a Steam fechada e com backup disponível.
