#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"
#include "colorize.h"
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

    // get the full path of the db file
    char *db_file = get_home_path(DB_FILE);

    // track the listing type
    bool list_all = false;
    // check if the user wants to see all tasks
    if (argc == 3 && strncasecmp(argv[2], "--all", strlen("--all")) == 0)
    {
        list_all = true;
    }
    // open the file in read mode
    FILE *file = fopen(db_file, "r");
    // check if we failed to open the file
    if (file == NULL)
    {
        printf("🚨 Could not open the file.");
        return 1;
    }
    char *line = malloc(MAX_LINE_LENGTH);
    int counter = 0;
    printf("#    ");
    printf("%-*s", MAX_TASK_NAME + 3, "Task Name");
    printf("%-15s", "Total");
    printf("%-27s", "Started");
    printf("%-27s\n", "Completed");
    printf("%*c\n", MAX_TASK_NAME + 30 + 30, ' ');
    // loop through lines inside the file
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        // check if this is the first line
        if (counter == 0)
        {
            // skip the first line
            counter++;
            // move on
            continue;
        }
        // get the line as a task struct
        struct Task *task = line_to_task(line);
        // check if this record's end_time is not set
        if (strcmp(task->end_date, END_TIME_PLACEHOLDER) == 0)
        {
            // string to time_t
            time_t start_t = (time_t)atol(task->id);
            // string to time_t
            time_t now = (time_t)atol(now_ts);
            // get start time as string
            char *start_time = get_datetime_from_timestamp(start_t);
            char *spent_time = get_relative_time(start_t, now);

            yellow();
            printf("%-5d", counter);
            cyan();
            // this is a completed task. print it.
            printf("%-*s", MAX_TASK_NAME + 3, task->task_name);
            green();
            printf("%-15s", spent_time);
            blue();
            printf("%-27s", start_time);
            green();
            printf("%-27s\n", "-");
            reset();

            // increase it
            counter++;
            free(start_time);
            free(spent_time);
        }
        free_task(task);
    }
    // user called list with --all argument
    if (list_all)
    {
        // let's seek to the start of the file
        fseek(file, 0, SEEK_SET);
        // set a counter for this loop
        int counter_a = 0;
        // again, loop through to the file
        while (fgets(line, MAX_LINE_LENGTH, file))
        {
            // yes, yes check if this the first line
            if (counter_a == 0)
            {
                // this is header line. let's skip.
                counter_a++;
                continue;
            }
            struct Task *task = line_to_task(line);
            // check if the end_date is NOT '??'
            if (strcmp(task->end_date, END_TIME_PLACEHOLDER))
            {
                // string to time_t
                time_t start_t = (time_t)atol(task->id);
                // string to time_t
                time_t end_t = (time_t)atol(task->end_date);
                // get start time as string
                char *start_time = get_datetime_from_timestamp(start_t);
                // get end time as string
                char *end_time = get_datetime_from_timestamp(end_t);

                char *spent_time = get_relative_time(start_t, end_t);

                yellow();
                printf("%-5d", counter);
                magenta();
                // this is a completed task. print it.
                printf("%-*s", MAX_TASK_NAME + 3, task->task_name);
                green();
                printf("%-15s", spent_time);
                blue();
                printf("%-27s", start_time);
                green();
                printf("%-27s\n", end_time);
                reset();
                counter++;
                free(start_time);
                free(end_time);
                free(spent_time);
            }
            free_task(task);
            counter_a++;
        }
    }
    free(line);
    fclose(file);
    return 0;
}