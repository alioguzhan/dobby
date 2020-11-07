#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>
#include "utils.h"
#include "colorize.h"

int create_db_file(void);
int create_config_file(void);
void strfcat(char *src, char *fmt, ...);

/** print the program usage */
void print_usage()
{
    green();
    printf("   Dobby %s\n", PACKAGE_VERSION);
    reset();
    yellow();
    printf("⭐️ Command line program to track and save your work / study activity.\n");
    printf("💡 Usage:\n");
    reset();
    printf("\tdobby start|stop task_name\n");
    printf("\tdobby list [--all]\n");
    printf("\tdobby --version|-v\n");
}

/** checks if the file exists at given PATH */
bool is_file_exists(const char *path)
{
    if (access(path, F_OK) != 0)
    {
        return false;
    }
    return true;
}

/** returns the $HOME path for the current user */
char *get_home_path(const char *file_path)
{
    char *home_dir_path = getenv(HOME_ENV);
    char *home_dir = malloc(strlen(home_dir_path) + 1);
    // TODO: check if allocation failed
    char *file;

    strcpy(home_dir, home_dir_path);

    file = malloc(strlen(home_dir) + strlen(file_path) + 1);
    // TODO: check if allocation failed
    strcpy(file, home_dir);
    strcat(file, file_path);
    free(home_dir);
    return file;
}

/** creates .dobby folder under the $HOME and puts some files in it. */
int prepare_config_files()
{
    char *config_dir = get_home_path(CONFIG_FOLDER);

    int ready = 1;

    if (!is_file_exists(config_dir))
    {
        printf("⏳ First time running. Creating required files and folders...\n");
        mkdir(config_dir, 0777);
        printf("✅ Done.\n");
    }
    create_db_file();
    create_config_file();

    free(config_dir);
    ready = 0;
    return ready;
}

/** creates a .csv file under the ~/.dobby AND writes the header line */
int create_db_file()
{
    char *db_file = get_home_path(DB_FILE);
    if (!is_file_exists(db_file))
    {
        FILE *db = fopen(db_file, "w");
        fputs("id,task_name,stopped_at\n", db);
        fclose(db);
    }
    free(db_file);
    return 0;
}

int create_config_file()
{
    char *config_file = get_home_path(CONFIG_FILE);
    if (!is_file_exists(config_file))
    {
        FILE *config = fopen(config_file, "w");
        fclose(config);
    }
    free(config_file);
    return 0;
}

char *get_datetime_from_timestamp(time_t ts)
{
    // get localtime from timestamp
    struct tm *dt = localtime(&ts);
    // init a buffer
    char *buffer = malloc(DATETIME_SIZE);
    // TODO: check if allocation failed
    // convert timestamp to datetime and write it to the buffer
    strftime(buffer, DATETIME_SIZE, "%c", dt);
    // return the human-readable date string
    return buffer;
}
struct Task *line_to_task(char *line)
{
    struct Task *task = malloc(sizeof(struct Task));
    // TODO: check if allocation failed
    const char *delim = ",";
    char *token;
    char *line_dup = strdup(line);
    // TODO: check if allocation failed

    task->raw_line = line_dup;

    token = strtok(line_dup, delim);
    task->id = token;
    // Token count in this line
    int t_count = 1;
    while (token != NULL)
    {
        token = strtok(NULL, delim);
        if (t_count == 1)
        {
            task->task_name = token;
        }
        else if (t_count == 2)
        {
            task->end_date = token;
        }
        t_count++;
    }
    return task;
}

char *get_relative_time(time_t start_time, time_t end_time)
{
    double seconds = difftime(end_time, start_time);
    int i = (int)seconds;
    int days, hours, mins, secs;
    days = hours = mins = secs = 0;
    char *result = calloc(1, 40);

    int DAY = 60 * 60 * 24;
    int HOUR = 60 * 60;
    int MIN = 60;
    while (i > 0)
    {
        if (i >= DAY)
        {
            i -= DAY;
            days++;
        }
        else if (i >= HOUR)
        {
            i -= HOUR;
            hours++;
        }
        else if (i >= MIN)
        {
            i -= MIN;
            mins++;
        }
        else
        {
            i -= 1;
            secs++;
        }
    }
    if (days > 0)
    {
        strfcat(result, "%dd ", days);
    }
    if (hours > 0)
    {
        strfcat(result, "%dh ", hours);
    }
    if (mins > 0)
    {
        strfcat(result, "%dm ", mins);
    }
    if (secs > 0)
    {
        strfcat(result, "%ds", secs);
    }
    return result;
}

// I have no idea how below works.
void strfcat(char *src, char *fmt, ...)
{
    size_t s = sizeof(src);
    char buf[s];
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    strcat(src, buf);
}

void free_task(struct Task *task)
{
    free(task->raw_line);
    free(task);
}
