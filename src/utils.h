// utils.h

/** Print program usage  */
void print_usage();

/** Returns a file path based on HOME directory */
const char *get_home_path(const char *file_path);

/** Check if file exists and writable for us */
bool is_file_exists(const char *path);

/** check config files and create them if not exist */
int prepare_config_files();