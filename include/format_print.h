#include <stdio.h>
#include <string.h>

#define STANDARD_COLOR "\x1B[0m"
#define BLUE_COLOR "\x1B[34m"
#define GREEN_COLOR "\x1B[32m"

enum COLORS
{
    STANDARD,
    BLUE,
    GREEN
};

static char *get_color(enum COLORS color)
{
    switch (color)
    {
    case STANDARD:
        return STANDARD_COLOR;
    case BLUE:
        return BLUE_COLOR;
    case GREEN:
        return GREEN_COLOR;
    default:
        puts("Incorrect color!");
        return "";
    }
}

void format_print(char *line, enum COLORS color)
{
    char *hex_color = get_color(color);

    printf("%s%s\n", hex_color, line);
}
