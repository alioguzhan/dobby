#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"
#include "colorize.h"
#include "stop.h"

int stop_task(int argc, char const *argv[])
{
    // generate unique id for the task from the timestamp
    // we will use this id for both identify the tasks and to figure out the start time, later
    time_t rawtime = time(NULL);
    struct tm *dt = localtime(&rawtime);
    char now_ts[64];
    strftime(now_ts, sizeof(now_ts), "%s", dt);

    // get the full path of the db file
    char *db_file = get_home_path(DB_FILE);
    // file pointer
    FILE *file;
    // to store file size
    size_t original_file_size;
    // open the file in read mode
    file = fopen(db_file, "r");

    // check if we failed to open the file
    if (file == NULL)
    {
        printf("🚨 Dobby could not open the file.");
        return 1;
    }

    // seek to the end of the file
    fseek(file, 0, SEEK_END);
    // get the file size
    original_file_size = ftell(file);
    // rollback to the start of the file
    fseek(file, 0, SEEK_SET);

    // allocate memory for the new file
    char *new_file = (char *)calloc(1, original_file_size);
    // TODO: check if allocation failed

    // store the total lines in the file
    int line_count = 0;
    char *line = NULL;
    // allocate memory for a single line
    line = malloc(MAX_LINE_LENGTH);
    // TODO: check if allocation failed
    // store the required byte amount for the new file
    size_t new_file_size = 0;
    // check if a task with given name is stopped
    bool stopped = false;

    // read lines through the file
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        // get the tokenized version of the line
        struct Task *task = line_to_task(line);
        // we are skipping the first line. it is the header line.
        if (line_count > 0)
        {
            if (strcasecmp(task->task_name, argv[2]) == 0 && strcasecmp(task->end_date, "??\n") == 0)
            {
                task->end_date = now_ts;
                // write the line with end date
                sprintf(line, "%s,%s,%s\n", task->id, task->task_name, task->end_date);
                if (!stopped)
                // if there are multiple tasks with the same name
                {
                    magenta();
                    printf("✅ %s ", task->task_name);
                    cyan();
                    // print completion message onec.
                    printf("is completed!\n");
                    reset();
                    stopped = true;
                }
            }
        }
        size_t line_size = strlen(line) + 1;
        line_count++;               // we are done with this line. let's move the next one.
        new_file_size += line_size; // increase the memory amount required for the new file

        if (new_file_size >= original_file_size) // check if the new file size is bigger than the original file size
        {
            new_file = (char *)realloc(new_file, new_file_size); // if so re allocate some memory
            // TODO: check if allocation failed
        }

        strncat(new_file, line, new_file_size); // append the line to new file
        free_task(task);                        // set it free
    }

    if (!stopped)
    {
        printf("🍄 A task with name");
        yellow();
        printf(" %s ", argv[2]);
        reset();
        printf("is not found\n");
    }

    fclose(file);                    // close the db file opened with 'read' mode.
    FILE *new = fopen(db_file, "w"); // open the same file to write
    fputs(new_file, new);            // write the new_file to the db file
    fclose(new);                     // close the file
    free(line);                      // no more lines. set it free
    free(new_file);                  // no more new_file. set it free

    return 0;
}