#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "colors.h"
#include "start.h"

int start_task(int argc, char const *argv[])
{
    // generate unique id for the task from the timestamp
    // we will use this id for both identify the tasks and to figure out the start time, later
    time_t rawtime = time(NULL);
    struct tm *dt = localtime(&rawtime);
    char now_ts[64];
    strftime(now_ts, sizeof(now_ts), "%s", dt);

    char *db_file = get_home_path(DB_FILE); // get the full path of the db file

    if (argc != 3) // check if task_name is provided
    {
        printf("💡 Usage: dobby start task_name\n");
        return 1;
    }
    if (strlen(argv[2]) > MAX_TASK_NAME) // check if task name length is valid
    {
        printf("✋ Task name length should be less than %d\n", MAX_TASK_NAME); // warn the user
        return 1;                                                             // then exit
    }
    if (strchr(argv[2], ',')) // check if task name contains , (comma)
    {
        printf("✋ Task name cannot contain ',' (comma)\n"); // warn th user
        return 1;                                           // then exit
    }

    FILE *f_ptr = fopen(db_file, "a");                                 // open the db file with append mode
    fprintf(f_ptr, "%s,%s,%s", now_ts, argv[2], END_TIME_PLACEHOLDER); // add new line for the task
    fclose(f_ptr);                                                     // close the file
    bold_cyan();
    printf("🚀 Dobby has started to work on "); // inform the user
    bold_magenta();
    printf("%s\n", argv[2]);
    reset();
    return 0;
}