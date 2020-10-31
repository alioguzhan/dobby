// utils.h

#define CONFIG_FOLDER "/.dobby/"
#define DB_FILE "/.dobby/dobby-db.csv"
#define CONFIG_FILE "/.dobby/dobby.cfg"
#define HOME_ENV "HOME"
#define DATETIME_SIZE 64

/** Print program usage  */
void print_usage(void);

/** Returns a file path based on HOME directory */
char *get_home_path(const char *file_path);

/** Check if file exists and writable for us */
bool is_file_exists(const char *path);

/** check config files and create them if not exist */
int prepare_config_files(void);

/** returns human-readable date-time string from timestamp */
char *get_datetime_from_timestamp(time_t ts);