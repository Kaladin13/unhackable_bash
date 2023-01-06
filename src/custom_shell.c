#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "../include/custom_shell.h"
#include "../include/format_print.h"
#include "../include/command_history.h"
#include "../include/utils.h"

#define CURRENT_PATH "."

const char *description_text =
    "Finally semester has ended...\n"
    "Now, after all my studying, I can develop my ultimate C-project\n"
    "I better make it safe as Igor has taught me: pie+canary+aslr enabled, try this you hacker!\n"
    "...but why the compiler keeps highlighting gets and printf with yellow???\n";

static void list_insides(char *path)
{
    DIR *current_dir = opendir(path);

    if (!current_dir)
    {
        return;
    }

    struct dirent *d;

    while ((d = readdir(current_dir)))
    {
        if (d->d_type != DT_DIR)
        {
            format_print(d->d_name, BLUE);
        }
        else
        {
            if (d->d_type == DT_DIR && strcmp(d->d_name, ".") && strcmp(d->d_name, ".."))
            {
                format_print(d->d_name, GREEN);

                // concat new path and run reccursively
                char new_path[255];
                sprintf(new_path, "%s/%s", path, d->d_name);

                list_insides(new_path);
            }
        }
    }

    closedir(current_dir);
}

void show_content()
{
    list_insides(CURRENT_PATH);

    // set color back to normal
    format_print("", STANDARD);
}

void description()
{
    puts(description_text);
}

void read_file()
{
    puts("Enter filename");

    char filename[63];
    gets(filename);

    show_file_content(filename);
}

void history(command_history *h)
{
    list_command_history(h);
}

void get_feedback()
{
    char feedback[64];

    puts("Now tell me about things you have learned");

    // completely safe
    fgets(feedback, sizeof(feedback), stdin);

    printf("Your feedback: ");
    printf(feedback);
}

void change_working_dir()
{   
    // intentionally unimplemented, not a real TODO
    // (parody for every student project ever)
    puts("TODO: implement this");
}