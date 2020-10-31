#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include "utils.h"

int create_db_file(void);
int create_config_file(void);

void print_usage()
{
    printf("\tDobby v0.1\n");
    printf("⭐️ Command line program to track and save your work / study activity.\n");
    printf("💡 Usage:\n\tdobby start|stop task_name\n");
    printf("\tdobby list [--all]\n");
}

bool is_file_exists(const char *path)
{
    if (access(path, F_OK) != 0)
    {
        return false;
    }
    return true;
}

char *get_home_path(const char *file_path)
{
    char *home_dir_path = getenv(HOME_ENV);
    char *home_dir = malloc(strlen(home_dir_path) + 1);
    assert(home_dir);
    char *file;

    strcpy(home_dir, home_dir_path);

    file = malloc(strlen(home_dir) + strlen(file_path) + 1);
    assert(file);
    strcpy(file, home_dir);
    strcat(file, file_path);
    free(home_dir);
    return file;
}

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
    struct tm *dt = localtime(&ts);            // get localtime from timestamp
    char *buffer = malloc(DATETIME_SIZE);      // init a buffer
    assert(buffer);                            // check if allocation is OK
    strftime(buffer, DATETIME_SIZE, "%c", dt); // convert timestamp to datetime and write it to the buffer
    printf("%ld == %s\n", ts, buffer);         // debug
    return buffer;                             // return the human-readable date string
}