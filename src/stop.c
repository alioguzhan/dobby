#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
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

    char *db_file = get_home_path(DB_FILE); // get the full path of the db file
    FILE *file;                             // file pointer
    long size;                              // to store file size
    file = fopen(db_file, "r");             // open the file in read mode
    if (file == NULL)                       // check if we failed to open the file
    {
        printf("🚨 Dobby could not open the file.");
        return 1;
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
            if (strcasecmp(task->task_name, argv[2]) == 0 && strcasecmp(task->end_date, "??\n") == 0)
            {
                task->end_date = now_ts;
                sprintf(line, "%s,%s,%s\n", task->id, task->task_name, task->end_date); // write the line with end date
                if (!stopped)
                { // if there are multiple tasks with the same name
                    magenta();
                    printf("✅ %s ", task->task_name);
                    cyan();
                    printf("is completed!\n"); // print completion message onec.
                    reset();
                    stopped = true;
                }
            }
        }
        line_count++;                      // we are done with this line. let's move the next one.
        new_file_size += strlen(line) + 1; // increase the memory amount required for the new file
        if (new_file_size >= size)         // check if the new file size is bigger than the original file size
        {
            new_file = realloc(new_file, new_file_size); // if so re allocate some memory
            assert(new_file);                            // check
        }
        strncat(new_file, line, strlen(line)); // append the line to new file
        free(task);                            // set it free
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