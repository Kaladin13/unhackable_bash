#include "../include/utils.h"

const char *menu[] = {
    "Show description",
    "List current directory's content",
    "Read file",
    "Show history",
    "Change working dir",
    "Give feedback",
};

int get_int_from_stdin()
{
    char buffer[MAX_DIGITS], *end_ptr;

    fgets(buffer, sizeof(buffer), stdin);

    int res = strtol(buffer, &end_ptr, 10);

    return res;
}

void print_menu()
{
    for (int i = 0; i < get_char_array_size(menu); i++)
    {
        printf("%d: %s\n", i + 1, menu[i]);
    }
}

void print_menu_item(int index)
{
    puts(menu[index - 1]);
}

void show_file_content(char *filename)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    if (buffer)
    {
        puts(buffer);
        free(buffer);
    }
}

void print_logo()
{
    show_file_content(LOGO_FILENAME);
}