#include <malloc.h>

#include "../include/command_history.h"
#include "../include/utils.h"

void add_command_history(int ch, command_history *history_list)
{
    if (ch >= 1 && ch <= 6)
    {
        command_history *last = history_list;

        while (last->next_command != NULL)
        {
            last = last->next_command;
        }

        last->next_command = (command_history *)malloc(sizeof(command_history));

        last->next_command->command = ch;
        last->next_command->next_command = NULL;
    }
}

void list_command_history(command_history *history_list)
{
    command_history *last = history_list;

    while (last != NULL)
    {
        print_menu_item(last->command);

        last = last->next_command;
    }

    printf("\n");
}