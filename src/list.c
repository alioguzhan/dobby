#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"
#include "colors.h"
#include "list.h"

// list tasks
int list_tasks(int argc, char const *argv[])
{
    // generate unique id for the task from the timestamp
    // we will use this id for both identify the tasks and to figure out the start time, later
    time_t rawtime = time(NULL);
    struct tm *dt = localtime(&rawtime);
    char now_ts[64];
    strftime(now_ts, sizeof(now_ts), "%s", dt);

    char *db_file = get_home_path(DB_FILE); // get the full path of the db file

    bool list_all = false;                                                // track the listing type
    if (argc == 3 && strncasecmp(argv[2], "--all", strlen("--all")) == 0) // check if the user wants to see all tasks
    {
        list_all = true;
    }
    FILE *file = fopen(db_file, "r"); // open the file in read mode
    if (file == NULL)                 // check if we failed to open the file
    {
        printf("🚨 Could not open the file.");
        return 1;
    }
    char *line = malloc(sizeof(char) * MAX_LINE_LENGTH);
    int counter = 0;
    printf("#    ");
    printf("%-*s", MAX_TASK_NAME + 3, "Task Name");
    printf("%-27s", "Started");
    printf("%-27s\n", "Completed");
    printf("%*c\n", MAX_TASK_NAME + 30 + 30, ' ');
    while (fgets(line, MAX_LINE_LENGTH, file)) // loop through lines inside the file
    {
        if (counter == 0) // check if this is the first line
        {
            counter++; // skip the first line
            continue;  // move on
        }
        struct Task *task = line_to_task(line);                // get the line as a task struct
        if (strcmp(task->end_date, END_TIME_PLACEHOLDER) == 0) // check if this record's end_time is not set
        {
            char *start_time = get_datetime_from_timestamp((time_t)atol(task->id)); // get start time as string
            bold_yellow();
            printf("%-5d", counter);
            bold_cyan();
            printf("%-*s", MAX_TASK_NAME + 3, task->task_name); // this is a completed task. print it.
            bold_blue();
            printf("%-27s", start_time);
            bold_green();
            printf("%-27s\n", "-");
            reset();

            counter++; // increase it
            free(start_time);
        }
    }
    if (list_all) // user called list with --all argument
    {
        fseek(file, 0, SEEK_SET);                  // let's seek to the start of the file
        int counter_a = 0;                         // set a counter for this loop
        while (fgets(line, MAX_LINE_LENGTH, file)) // again, loop through to the file
        {
            if (counter_a == 0) // yes, yes check if this the first line
            {
                counter_a++; // this is header line. let's skip.
                continue;
            }
            struct Task *task = line_to_task(line);
            if (strcmp(task->end_date, END_TIME_PLACEHOLDER)) // check if the end_date is NOT '??'
            {
                char *start_time = get_datetime_from_timestamp((time_t)atol(task->id));     // get start time as string
                char *end_time = get_datetime_from_timestamp((time_t)atol(task->end_date)); // get end time as string
                bold_yellow();
                printf("%-5d", counter);
                bold_magenta();
                printf("%-*s", MAX_TASK_NAME + 3, task->task_name); // this is a completed task. print it.
                bold_blue();
                printf("%-27s", start_time);
                bold_green();
                printf("%-27s\n", end_time);
                reset();
                counter++;
            }
            counter_a++;
        }
    }
    free(line);
    fclose(file);
    return 0;
}