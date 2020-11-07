#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "start.h"
#include "list.h"
#include "stop.h"

int main(int argc, char const *argv[])
{
    // check arg count
    if (argc == 1)
    {
        print_usage();
        return 1;
    }

    // check if program files is created under $HOME
    if (prepare_config_files() != 0)
    {
        printf("🚫 Error while creating config files.");
        return 1;
    }
    // user starting a new task
    if (strncasecmp(argv[1], START_PARAM, strlen(START_PARAM)) == 0)
    {
        return start_task(argc, argv);
    } // user stopping a task
    else if (strncasecmp(argv[1], STOP_PARAM, strlen(STOP_PARAM)) == 0)
    {
        return stop_task(argc, argv);
    } // user wants to see the list of tasks
    else if (strncasecmp(argv[1], LIST_PARAM, strlen(LIST_PARAM)) == 0)
    {
        return list_tasks(argc, argv);
    }
    else if (strcasecmp(argv[1], "--version") == 0 || strcasecmp(argv[1], "-v") == 0)
    {
        printf("%s\n", PACKAGE_STRING);
        return 0;
    }
    else
    {
        // Nothing matches. Print usage...
        print_usage();
        return 1;
    }
}
