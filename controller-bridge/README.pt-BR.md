# Steam Media Pack Controller Bridge

## Objetivo

O Controller Bridge é um componente experimental do Steam Media Pack responsável por criar um mouse virtual utilizando `uinput` e controlar esse mouse através de um controle compatível com SDL2.

A motivação deste projeto é eliminar a dependência do Steam Input para navegação nas plataformas de streaming, proporcionando um comportamento consistente entre serviços como Netflix, Disney+, Max, Prime Video, Spotify e YouTube.

## Funcionalidades previstas

- Mouse virtual via Linux uinput
- Compatibilidade com SDL2 GameController API
- Reconexão automática de controles
- Movimento do cursor pelo analógico direito
- Clique esquerdo, direito e botão do meio
- Rolagem utilizando os gatilhos
- Execução em segundo plano como serviço de usuário (systemd)

## Status

**Experimental**

Este componente ainda está em desenvolvimento e não faz parte da versão estável do Steam Media Pack.

## Compilação

```bash
make
```

## Limpeza

```bash
make clean
```
