// colorize.h

#define RED "\33[1;31m"
#define GREEN "\33[1;32m"
#define BLUE "\33[1;34m"
#define CYAN "\33[1;36m"
#define MAGENTA "\33[1;35m"
#define YELLOW "\33[1;33m"
#define RESET "\33[0m"

/** sets the output color to BOLD RED */
void red(void);

/** sets the output color to BOLD GREEN */
void green(void);

/** sets the output color to BOLD BLUE */
void blue(void);

/** sets the output color to BOLD YELLO */
void yellow(void);

/** sets the output color to BOLD MAGENTA */
void magenta(void);

/** sets the output color to BOLD CYAN */
void cyan(void);

/** Resets the output color */
void reset(void);
