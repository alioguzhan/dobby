#include <stdio.h>

void red(void)
{
    printf("\33[0;31m");
}

void bold_red(void)
{
    printf("\33[1;31m");
}

void green(void)
{
    printf("\33[0;32m");
}

void bold_green(void)
{
    printf("\33[1;32m");
}

void blue(void)
{
    printf("\33[0;34m");
}

void bold_blue(void)
{
    printf("\33[1;34m");
}

void yellow(void)
{
    printf("\33[0;33m");
}

void bold_yellow(void)
{
    printf("\33[1;33m");
}

void magenta(void)
{
    printf("\33[0;35m");
}

void bold_magenta(void)
{
    printf("\33[1;35m");
}

void cyan(void)
{
    printf("\33[0;36m");
}

void bold_cyan(void)
{
    printf("\33[1;36m");
}

void reset(void)
{
    printf("\33[0m");
}
