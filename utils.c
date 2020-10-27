#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CONFIG_FOLDER "/.dobby/"
#define DB_FILE "/.dobby/db"
#define CONFIG_FILE "/.dobby/config"
#define HOME_ENV "HOME"

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

const char *get_home_path(const char *file_path)
{
    char *home_dir = malloc(strlen(getenv(HOME_ENV)));
    char *file;

    strcpy(home_dir, getenv(HOME_ENV));

    file = malloc(strlen(home_dir) + strlen(file_path) + 1);
    strcpy(file, home_dir);
    strcat(file, file_path);
    return file;
}

int prepare_config_files()
{
    const char *config_dir = get_home_path(CONFIG_FOLDER);
    const char *db_file = get_home_path(DB_FILE);
    const char *config_file = get_home_path(CONFIG_FILE);

    int ready = 1;

    if (!is_file_exists(config_dir))
    {
        printf("⏳ First time running. Creating required files and folders...\n");
        mkdir(config_dir, 0777);

        FILE *db = fopen(db_file, "w");
        fclose(db);

        FILE *config = fopen(config_file, "w");
        fclose(config);

        printf("✅ Done.\n");
    }
    else
    {
        if (!is_file_exists(db_file))
        {
            FILE *db = fopen(db_file, "w");
            fclose(db);
        }
        if (!is_file_exists(config_file))
        {
            FILE *config = fopen(config_file, "w");
            fclose(config);
        }
    }

    ready = 0;
    return ready;
}