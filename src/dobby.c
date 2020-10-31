#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"

#define START_PARAM "start"
#define STOP_PARAM "stop"
#define LIST_PARAM "list"

typedef struct Task
{
    char *id;
    char *task_name;
    char *start_date;
    char *end_date;
} task;

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
        printf("🚫 Error while creating config files.");
        return 1;
    }
    // generate unique id for the task from the timestamp
    time_t rawtime = time(NULL);
    struct tm *dt = localtime(&rawtime);
    char id[64];
    strftime(id, sizeof(id), "%s", dt);

    char *db_file = get_home_path(DB_FILE); // get the full path of the db file

    if (strncasecmp(argv[1], START_PARAM, strlen(START_PARAM)) == 0)
    {
        if (argc != 3)
        {
            printf("💡 Usage: dobby start task_name\n");
            return 1;
        }

        FILE *f_ptr = fopen(db_file, "a");
        fprintf(f_ptr, "%s,%s,??\n", id, argv[2]);
        fclose(f_ptr);
        printf("🚀 Started to work on %s.\n", argv[2]);
    }
    else if (strncasecmp(argv[1], STOP_PARAM, strlen(STOP_PARAM)) == 0)
    {
        FILE *file;
        long size;
        file = fopen(db_file, "r");
        if (file == NULL)
        {
            printf("🚨 Could not open the file.");
            return 0;
        }
        fseek(file, 0, SEEK_END);
        // get the file size
        size = ftell(file);
        fseek(file, 0, SEEK_SET);
        // prepare lines array with file size ?
        printf("File size: %lu\n", size);

        char *new_file = (char *)malloc(sizeof(char) * size);
        assert(new_file);

        int ch;
        int ch_count = 0;
        int line_count = 0;
        int line_length = 0;
        int STARTING_MAX = 20;
        char *line = malloc(sizeof(char) * STARTING_MAX);
        assert(line);

        while ((ch = getc(file)) != EOF)
        {
            if (line_length == STARTING_MAX)
            {
                line = realloc(line, sizeof(char) * STARTING_MAX * 2);
                if (line == NULL)
                {
                    perror("🚨 Out of memory while reading lines\n");
                    exit(1);
                }
            }
            if (ch == '\n')
            {

                printf("Line %d: %s\n", line_count, line);
                if (line_count > 0) // we are skipping the first line.
                {
                    /** TODO
                     * Split the line by comma (,)
                     * read them into task struct, 
                     * check if the task_name is same as with command-line param
                     * if it is set the task->end_data = now
                     * else move on
                     */
                }
                line_count++;
                free(line);
                line = malloc(sizeof(char) * STARTING_MAX);
                assert(line);

                line_length = 0;
                STARTING_MAX = 20;
            }
            else
            {
                line[line_length] = ch;
                line_length++;
            }
            new_file[ch_count] = ch;
            ch_count++;
        }

        free(line);

        printf("new file size: %lu\n", strlen(new_file));
        printf("new file: %s\n", new_file);

        FILE *new = fopen(db_file, "w");
        fputs(new_file, new);
        // clean up
        fclose(new);
        fclose(file);
        free(new_file);
        char *sd = get_datetime_from_timestamp(1604099414);
        printf("Start Date: %s\n", sd);
        free(sd);
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
        print_usage(); // Nothing matches. Print usage...
        return 1;
    }
    free(db_file);
    return 0;
}
