#ifndef SMP_LOG_H
#define SMP_LOG_H

#include <stdarg.h>

typedef enum
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NONE
} LogLevel;

/*
 * Define o nível mínimo exibido.
 *
 * Exemplo:
 * log_set_level(LOG_LEVEL_INFO);
 *
 * Nesse caso, mensagens DEBUG serão ignoradas.
 */
void log_set_level(LogLevel level);

LogLevel log_get_level(void);

/*
 * Função interna utilizada pelas macros.
 * Normalmente não deve ser chamada diretamente.
 */
void log_write(
    LogLevel level,
    const char *file,
    int line,
    const char *format,
    ...);

#define LOG_DEBUG(...) \
    log_write(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_INFO(...) \
    log_write(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_WARN(...) \
    log_write(LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_ERROR(...) \
    log_write(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#endif