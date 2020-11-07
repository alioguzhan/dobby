#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "colorize.h"
#include "start.h"

int start_task(int argc, char const *argv[])
{
    // generate unique id for the task from the timestamp
    // we will use this id for both identify the tasks and to figure out the start time, later
    time_t rawtime = time(NULL);
    struct tm *dt = localtime(&rawtime);
    char now_ts[64];
    strftime(now_ts, sizeof(now_ts), "%s", dt);

    // get the full path of the db file
    char *db_file = get_home_path(DB_FILE);

    // check if task_name is provided
    if (argc != 3)
    {
        printf("💡 Usage: dobby start task_name\n");
        return 1;
    }
    // check if task name length is valid
    if (strlen(argv[2]) > MAX_TASK_NAME)
    {
        // warn the user
        printf("✋ Task name length should be less than %d\n", MAX_TASK_NAME);
        // then exit
        return 1;
    }
    // check if task name contains , (comma)
    if (strchr(argv[2], ','))
    {
        // warn th user
        printf("✋ Task name cannot contain ',' (comma)\n");
        // then exit
        return 1;
    }

    // open the db file with append mode
    FILE *f_ptr = fopen(db_file, "a");
    // check if we failed to open the file
    if (f_ptr == NULL)
    {
        fprintf(stderr, "🚨 Could not open the db file while starting a task.\n");
        return 1;
    }
    // add new line for the task
    int write_to_file = fprintf(f_ptr, "%s,%s,%s", now_ts, argv[2], END_TIME_PLACEHOLDER);
    // fprintf returns the number of bytes that are printed.
    // OR a negative value if there is an error
    if (write_to_file < 0)
    {
        fprintf(stderr, "🚨 Error while adding a new line to db file.\n");
        fclose(f_ptr);
        return 1;
    }

    // close the file
    fclose(f_ptr);
    cyan();
    // inform the user
    printf("🚀 Dobby has started to work on ");
    magenta();
    printf("%s\n", argv[2]);
    reset();
    return 0;
}