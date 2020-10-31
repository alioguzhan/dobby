#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include "utils.h"

#define START_PARAM "start"
#define STOP_PARAM "stop"
#define LIST_PARAM "list"
#define MAX_TASK_NAME 30
#define MAX_LINE_LENGTH 55

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
        printf("🚫 Error while creating config files.");
        return 1;
    }
    // generate unique id for the task from the timestamp
    // we will use this id for both identify the tasks and to figure out the start time, later
    time_t rawtime = time(NULL);
    struct tm *dt = localtime(&rawtime);
    char now_ts[64];
    strftime(now_ts, sizeof(now_ts), "%s", dt);

    char *db_file = get_home_path(DB_FILE); // get the full path of the db file

    if (strncasecmp(argv[1], START_PARAM, strlen(START_PARAM)) == 0) // user starting a new task
    {
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

        FILE *f_ptr = fopen(db_file, "a");             // open the db file with append mode
        fprintf(f_ptr, "%s,%s,??\n", now_ts, argv[2]); // add new line for the task
        fclose(f_ptr);                                 // close the file
        printf("🚀 Started to work on %s.\n", argv[2]); // inform the user
    }
    else if (strncasecmp(argv[1], STOP_PARAM, strlen(STOP_PARAM)) == 0) // user stopping a task
    {
        FILE *file;                 // file pointer
        long size;                  // to store file size
        file = fopen(db_file, "r"); // open the file in read mode
        if (file == NULL)           // check if we failed to open the file
        {
            printf("🚨 Could not open the file.");
            return 0;
        }
        fseek(file, 0, SEEK_END); // seek to the end of the file
        size = ftell(file);       // get the file size
        fseek(file, 0, SEEK_SET); // rollback to the start of the file
        // printf("File size: %lu\n", size);

        char *new_file = (char *)malloc(sizeof(char) * size); // allocate memory for the new file
        assert(new_file);                                     // check if allocation failed

        int line_count = 0;                                  // store the total lines in the file
        char *line = malloc(sizeof(char) * MAX_LINE_LENGTH); // allocate memory for a single line
        assert(line);                                        // check if allocation failed
        long new_file_size = 0;                              // store the required byte amount for the new file
        bool stopped = false;                                // check if a task with given name is stopped
        while (fgets(line, MAX_LINE_LENGTH, file))           // read lines through the file
        {
            struct Task *task = line_to_task(line); // get the tokenized version of the line
            if (line_count > 0)                     // we are skipping the first line. it is the header line.
            {
                if (strcasecmp(task->task_name, argv[2]) == 0)
                {
                    task->end_date = now_ts;
                    sprintf(line, "%s,%s,%s\n", task->id, task->task_name, task->end_date); // write the line with end date
                    if (!stopped)
                    {                                                   // if there are multiple tasks with the same name
                        printf("✅ %s is completed\n", task->task_name); // print completion message onec.
                        stopped = true;
                    }
                }
            }
            line_count++;                  // we are done with this line. let's move the next one.
            new_file_size += strlen(line); // increase the memory amount required for the new file
            if (new_file_size >= size)     // check if the new file size is bigger than the original file size
            {
                new_file = realloc(new_file, new_file_size); // if so re allocate some memory
                assert(new_file);                            // check
            }
            strncat(new_file, line, strlen(line)); // append the line to new file
            free(task);                            // set it free
        }

        fclose(file);                    // close the db file opened with 'read' mode.
        FILE *new = fopen(db_file, "w"); // open the same file to write
        fputs(new_file, new);            // write the new_file to the db file
        fclose(new);                     // close the file
        free(line);                      // no more lines. set it free
        free(new_file);                  // no more new_file. set it free
    }
    else if (strncasecmp(argv[1], LIST_PARAM, strlen(LIST_PARAM)) == 0) // user wants to see the list of tasks
    {
        bool list_all = false;                                               // track the listing type
        if (argc == 3 && strncasecmp(argv[2], "--all", strlen("-all")) == 0) // check if the user wants to see all tasks
        {
            list_all = true;
            printf("TODO: list all tasks\n");
        }
        else
        {
            // just display all ongoing tasksÎ
            printf("TODO: list all active tasks\n");
        }
    }
    else
    {
        print_usage(); // Nothing matches. Print usage...
        return 1;
    }
    free(db_file);
    return 0;
}
