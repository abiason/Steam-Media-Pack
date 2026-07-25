#include "config_parser.h"

#include "config.h"
#include "log.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Remove espaços em branco do início e do final
 * de uma string.
 *
 * A função altera a própria string recebida.
 */
static char *trim(char *text)
{
    if (text == NULL)
    {
        return NULL;
    }

    while (isspace((unsigned char)*text))
    {
        ++text;
    }

    if (*text == '\0')
    {
        return text;
    }

    char *end = text + strlen(text) - 1;

    while (end > text &&
           isspace((unsigned char)*end))
    {
        --end;
    }

    end[1] = '\0';

    return text;
}

/*
 * Limita um valor inteiro ao intervalo informado.
 */
static int clamp_int(
    int value,
    int minimum,
    int maximum)
{
    if (value < minimum)
    {
        return minimum;
    }

    if (value > maximum)
    {
        return maximum;
    }

    return value;
}

/*
 * Limita um valor de ponto flutuante
 * ao intervalo informado.
 */
static float clamp_float(
    float value,
    float minimum,
    float maximum)
{
    if (value < minimum)
    {
        return minimum;
    }

    if (value > maximum)
    {
        return maximum;
    }

    return value;
}

/*
 * Converte uma string para inteiro.
 *
 * Diferentemente de atoi(), esta função consegue
 * detectar valores inválidos e estouro numérico.
 */
static bool parse_int(
    const char *text,
    int *result)
{
    if (text == NULL ||
        result == NULL ||
        *text == '\0')
    {
        return false;
    }

    errno = 0;

    char *end = NULL;

    long value = strtol(
        text,
        &end,
        10);

    if (errno == ERANGE ||
        value < INT_MIN ||
        value > INT_MAX)
    {
        return false;
    }

    if (end == text)
    {
        return false;
    }

    while (isspace((unsigned char)*end))
    {
        ++end;
    }

    if (*end != '\0')
    {
        return false;
    }

    *result = (int)value;

    return true;
}

/*
 * Converte uma string para float.
 *
 * Detecta texto inválido e valores fora
 * do intervalo representável.
 */
static bool parse_float(
    const char *text,
    float *result)
{
    if (text == NULL ||
        result == NULL ||
        *text == '\0')
    {
        return false;
    }

    errno = 0;

    char *end = NULL;

    float value = strtof(
        text,
        &end);

    if (errno == ERANGE)
    {
        return false;
    }

    if (end == text)
    {
        return false;
    }

    while (isspace((unsigned char)*end))
    {
        ++end;
    }

    if (*end != '\0')
    {
        return false;
    }

    *result = value;

    return true;
}

/*
 * Registra quando um valor inteiro precisou
 * ser limitado ao intervalo permitido.
 */
static void log_int_adjustment(
    const char *key,
    int original,
    int adjusted)
{
    if (original == adjusted)
    {
        return;
    }

    LOG_WARN(
        "%s fora do intervalo permitido: %d. "
        "Valor ajustado para %d.",
        key,
        original,
        adjusted);
}

/*
 * Registra quando um valor float precisou
 * ser limitado ao intervalo permitido.
 */
static void log_float_adjustment(
    const char *key,
    float original,
    float adjusted)
{
    if (original == adjusted)
    {
        return;
    }

    LOG_WARN(
        "%s fora do intervalo permitido: %.3f. "
        "Valor ajustado para %.3f.",
        key,
        original,
        adjusted);
}

/*
 * Aplica uma propriedade inteira à configuração global.
 *
 * Retorna false quando a chave não pertence ao grupo
 * de propriedades inteiras.
 */
static bool set_int(
    const char *key,
    int value)
{
    int validated = value;

    if (strcmp(key, "cursor.deadzone") == 0)
    {
        validated =
            clamp_int(value, 0, 32767);

        log_int_adjustment(
            key,
            value,
            validated);

        g_config.cursor_deadzone = validated;

        return true;
    }

    if (strcmp(key, "cursor.max_speed") == 0)
    {
        validated =
            clamp_int(value, 1, 100);

        log_int_adjustment(
            key,
            value,
            validated);

        g_config.cursor_max_speed = validated;

        return true;
    }

    if (strcmp(key, "scroll.initial_delay") == 0)
    {
        validated =
            clamp_int(value, 0, 5000);

        log_int_adjustment(
            key,
            value,
            validated);

        g_config.scroll_initial_delay_ms =
            (unsigned int)validated;

        return true;
    }

    if (strcmp(key, "scroll.repeat_interval") == 0)
    {
        validated =
            clamp_int(value, 1, 1000);

        log_int_adjustment(
            key,
            value,
            validated);

        g_config.scroll_repeat_interval_ms =
            (unsigned int)validated;

        return true;
    }

    if (strcmp(key, "scroll.step") == 0)
    {
        validated =
            clamp_int(value, 1, 20);

        log_int_adjustment(
            key,
            value,
            validated);

        g_config.scroll_step = validated;

        return true;
    }

    return false;
}

/*
 * Aplica uma propriedade de ponto flutuante
 * à configuração global.
 *
 * Retorna false quando a chave não pertence
 * ao grupo de propriedades float.
 */
static bool set_float(
    const char *key,
    float value)
{
    float validated = value;

    if (strcmp(key, "cursor.exponent") == 0)
    {
        validated =
            clamp_float(value, 1.0f, 5.0f);

        log_float_adjustment(
            key,
            value,
            validated);

        g_config.cursor_exponent = validated;

        return true;
    }

    if (strcmp(key, "cursor.sensitivity") == 0)
    {
        validated =
            clamp_float(value, 0.1f, 10.0f);

        log_float_adjustment(
            key,
            value,
            validated);

        g_config.cursor_sensitivity = validated;

        return true;
    }

    if (strcmp(key, "cursor.smoothing") == 0)
    {
        validated =
            clamp_float(value, 0.0f, 0.99f);

        log_float_adjustment(
            key,
            value,
            validated);

        g_config.cursor_smoothing = validated;

        return true;
    }

    return false;
}

/*
 * Verifica se a chave pertence às configurações
 * de ponto flutuante.
 */
static bool is_float_key(const char *key)
{
    return
        strcmp(key, "cursor.exponent") == 0 ||
        strcmp(key, "cursor.sensitivity") == 0 ||
        strcmp(key, "cursor.smoothing") == 0;
}

/*
 * Verifica se a chave pertence às configurações inteiras.
 */
static bool is_int_key(const char *key)
{
    return
        strcmp(key, "cursor.deadzone") == 0 ||
        strcmp(key, "cursor.max_speed") == 0 ||
        strcmp(key, "scroll.initial_delay") == 0 ||
        strcmp(key, "scroll.repeat_interval") == 0 ||
        strcmp(key, "scroll.step") == 0;
}

/*
 * Descarta o restante de uma linha que excedeu
 * CONFIG_MAX_LINE_LENGTH.
 */
static void discard_line_remainder(FILE *file)
{
    int character = 0;

    while ((character = fgetc(file)) != '\n' &&
           character != EOF)
    {
        /*
         * Apenas descarta os caracteres restantes.
         */
    }
}

/*
 * Processa uma única propriedade no formato:
 *
 * chave=valor
 */
static void process_property(
    char *text,
    unsigned int line_number)
{
    char *separator = strchr(text, '=');

    if (separator == NULL)
    {
        LOG_WARN(
            "Linha %u inválida: separador '=' não encontrado.",
            line_number);

        return;
    }

    *separator = '\0';

    char *key = trim(text);
    char *value_text = trim(separator + 1);

    if (key == NULL ||
        value_text == NULL ||
        *key == '\0' ||
        *value_text == '\0')
    {
        LOG_WARN(
            "Linha %u inválida: chave ou valor ausente.",
            line_number);

        return;
    }

    if (!is_int_key(key) &&
        !is_float_key(key))
    {
        LOG_WARN(
            "Linha %u: configuração desconhecida: %s.",
            line_number,
            key);

        return;
    }

    if (is_int_key(key))
    {
        int value = 0;

        if (!parse_int(
                value_text,
                &value))
        {
            LOG_WARN(
                "Linha %u: valor inteiro inválido para %s: %s.",
                line_number,
                key,
                value_text);

            return;
        }

        set_int(
            key,
            value);

        LOG_DEBUG(
            "Configuração aplicada: %s = %d.",
            key,
            value);

        return;
    }

    float value = 0.0f;

    if (!parse_float(
            value_text,
            &value))
    {
        LOG_WARN(
            "Linha %u: valor decimal inválido para %s: %s.",
            line_number,
            key,
            value_text);

        return;
    }

    set_float(
        key,
        value);

    LOG_DEBUG(
        "Configuração aplicada: %s = %.3f.",
        key,
        value);
}

bool config_load(const char *path)
{
    if (path == NULL ||
        *path == '\0')
    {
        LOG_ERROR(
            "Caminho do arquivo de configuração inválido.");

        return false;
    }

    FILE *file = fopen(
        path,
        "r");

    if (file == NULL)
    {
        LOG_WARN(
            "Não foi possível abrir o arquivo de configuração "
            "%s: %s.",
            path,
            strerror(errno));

        return false;
    }

    LOG_INFO(
        "Carregando configurações de: %s.",
        path);

    char line[CONFIG_MAX_LINE_LENGTH];
    unsigned int line_number = 0;

    while (fgets(
               line,
               sizeof(line),
               file) != NULL)
    {
        ++line_number;

        /*
         * Se não houver quebra de linha e ainda não estivermos
         * no final do arquivo, a linha excedeu o tamanho máximo.
         */
        if (strchr(line, '\n') == NULL &&
            !feof(file))
        {
            LOG_WARN(
                "Linha %u excede o limite de %d caracteres "
                "e será ignorada.",
                line_number,
                CONFIG_MAX_LINE_LENGTH - 1);

            discard_line_remainder(file);

            continue;
        }

        char *text = trim(line);

        /*
         * Ignora linhas vazias e comentários.
         */
        if (text == NULL ||
            *text == '\0' ||
            *text == '#')
        {
            continue;
        }

        process_property(
            text,
            line_number);
    }

    if (ferror(file))
    {
        LOG_ERROR(
            "Erro durante a leitura do arquivo de configuração: %s.",
            path);

        fclose(file);

        return false;
    }

    fclose(file);

    LOG_INFO(
        "Arquivo de configuração processado com sucesso.");

    return true;
}