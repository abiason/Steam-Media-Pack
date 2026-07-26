#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

/*
 * Opções obtidas a partir da linha de comando.
 */
typedef struct
{
    /*
     * Caminho do arquivo de configuração.
     *
     * Quando NULL, o programa utilizará
     * CONFIG_DEFAULT_PATH.
     */
    const char *config_path;

    /*
     * Exibe a ajuda.
     */
    bool show_help;

    /*
     * Exibe a versão.
     */
    bool show_version;

} CliOptions;

/*
 * Inicializa a estrutura com os valores padrão.
 */
void cli_init(
    CliOptions *options);

/*
 * Processa os argumentos da linha de comando.
 *
 * Retorna:
 *
 * true  -> argumentos válidos.
 * false -> argumento inválido.
 */
bool cli_parse(
    CliOptions *options,
    int argc,
    char **argv);

/*
 * Exibe a ajuda do programa.
 */
void cli_print_help(
    const char *program_name);

/*
 * Exibe a versão do programa.
 */
void cli_print_version(void);

#endif