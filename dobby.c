#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

#define START_PARAM "start"
#define STOP_PARAM "stop"
#define LIST_PARAM "list"

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        print_usage();
        return 1;
    }
    // check if program files is created under $HOME
    if (prepare_config_files() != 0)
    {
        // TODO: Show a better error message here ?
        printf("⚠️ Error while creating config files.");
        return 1;
    }

    if (strncasecmp(argv[1], START_PARAM, strlen(START_PARAM)) == 0)
    {
        printf("TODO: start a task \n");
    }
    else if (strncasecmp(argv[1], STOP_PARAM, strlen(STOP_PARAM)) == 0)
    {
        printf("TODO: stop a task\n");
    }
    else if (strncasecmp(argv[1], LIST_PARAM, strlen(LIST_PARAM)) == 0)
    {
        bool list_all = false;
        if (argc == 3 && strncasecmp(argv[2], "--all", strlen("-all")) == 0)
        {
            list_all = true;
            printf("TODO: list all tasks\n");
        }
        else
        {
            printf("TODO: list all active tasks\n");
        }
    }
    else
    {
        // Nothing matches. Print usage...
        print_usage();
        return 1;
    }
    return 0;
}
