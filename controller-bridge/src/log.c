#include "log.h"

#include <stdio.h>
#include <time.h>

static LogLevel current_level = LOG_LEVEL_INFO;

static const char *log_level_to_string(LogLevel level)
{
    switch (level)
    {
        case LOG_LEVEL_DEBUG:
            return "DEBUG";

        case LOG_LEVEL_INFO:
            return "INFO";

        case LOG_LEVEL_WARN:
            return "WARN";

        case LOG_LEVEL_ERROR:
            return "ERROR";

        case LOG_LEVEL_NONE:
            return "NONE";

        default:
            return "UNKNOWN";
    }
}

static const char *get_filename(const char *path)
{
    const char *filename = path;

    if (path == NULL)
        return "unknown";

    for (const char *cursor = path; *cursor != '\0'; ++cursor)
    {
        if (*cursor == '/' || *cursor == '\\')
            filename = cursor + 1;
    }

    return filename;
}

static void format_timestamp(
    char *buffer,
    size_t buffer_size)
{
    time_t now = time(NULL);
    struct tm local_time;

    if (localtime_r(&now, &local_time) == NULL)
    {
        snprintf(
            buffer,
            buffer_size,
            "0000-00-00 00:00:00");

        return;
    }

    if (strftime(
            buffer,
            buffer_size,
            "%Y-%m-%d %H:%M:%S",
            &local_time) == 0)
    {
        snprintf(
            buffer,
            buffer_size,
            "0000-00-00 00:00:00");
    }
}

void log_set_level(LogLevel level)
{
    if (level < LOG_LEVEL_DEBUG ||
        level > LOG_LEVEL_NONE)
    {
        return;
    }

    current_level = level;
}

LogLevel log_get_level(void)
{
    return current_level;
}

void log_write(
    LogLevel level,
    const char *file,
    int line,
    const char *format,
    ...)
{
    char timestamp[20];
    va_list arguments;

    if (format == NULL)
        return;

    if (level < current_level ||
        current_level == LOG_LEVEL_NONE)
    {
        return;
    }

    format_timestamp(
        timestamp,
        sizeof(timestamp));

    /*
     * Todos os logs usam stderr.
     *
     * Isso facilita a captura pelo systemd/journald
     * e evita perda de mensagens devido ao buffering.
     */
    fprintf(
        stderr,
        "[%s] [%-5s] [%s:%d] ",
        timestamp,
        log_level_to_string(level),
        get_filename(file),
        line);

    va_start(arguments, format);
    vfprintf(stderr, format, arguments);
    va_end(arguments);

    fputc('\n', stderr);
    fflush(stderr);
}