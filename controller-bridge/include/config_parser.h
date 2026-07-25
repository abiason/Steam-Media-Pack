#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdbool.h>

/*
 * Arquivo de configuração padrão.
 */
#define CONFIG_DEFAULT_PATH "controller.conf"

/*
 * Tamanho máximo de uma linha do arquivo
 * de configuração.
 */
#define CONFIG_MAX_LINE_LENGTH 256

/*
 * Carrega as configurações do arquivo informado.
 *
 * As configurações válidas são aplicadas diretamente
 * à estrutura global g_config.
 *
 * Retorna:
 *
 * true:
 *   O arquivo foi aberto e processado.
 *
 * false:
 *   O caminho é inválido ou o arquivo não pôde ser aberto.
 *
 * Linhas inválidas são ignoradas e registradas no log,
 * mantendo os valores atuais de g_config.
 */
bool config_load(const char *path);

#endif