#include "cli.h"

#include <stdio.h>
#include <string.h>

#include "config_parser.h"
#include "version.h"

void cli_init(CliOptions *options)
{
    if (options == NULL)
    {
        return;
    }

    options->config_path = CONFIG_DEFAULT_PATH;
    options->show_help = false;
    options->show_version = false;
}

bool cli_parse(
    CliOptions *options,
    int argc,
    char **argv)
{
    int i;

    if (options == NULL)
    {
        return false;
    }

    for (i = 1; i < argc; i++)
    {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0 ||
            strcmp(arg, "-h") == 0)
        {
            options->show_help = true;
        }
        else if (strcmp(arg, "--version") == 0 ||
                 strcmp(arg, "-v") == 0)
        {
            options->show_version = true;
        }
        else if (strcmp(arg, "--config") == 0 ||
                 strcmp(arg, "-c") == 0)
        {
            if (i + 1 >= argc)
            {
                fprintf(stderr,
                        "Missing value for option '%s'.\n",
                        arg);
                return false;
            }

            options->config_path = argv[++i];
        }
        else
        {
            fprintf(stderr,
                    "Unknown option: %s\n",
                    arg);
            return false;
        }
    }

    return true;
}

void cli_print_help(const char *program_name)
{
    printf(
        "Usage: %s [OPTIONS]\n"
        "\n"
        "Options:\n"
        "  -h, --help              Show this help message\n"
        "  -v, --version           Show program version\n"
        "  -c, --config <file>     Configuration file\n",
        program_name);
}

void cli_print_version(void)
{
    printf("%s\n", SMP_CONTROLLER_VERSION);
}