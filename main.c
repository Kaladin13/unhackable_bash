#include <stdio.h>
#include <malloc.h>

#include "include/utils.h"
#include "include/custom_shell.h"
#include "include/command_history.h"

int main(int argc, char const *argv[])
{
    command_history *history_list = (command_history *)malloc(sizeof(command_history));
    history_list->command = 1;
    history_list->next_command = NULL;

    print_logo();

    while (1)
    {
        print_menu();
        int ch = get_int_from_stdin();

        switch (ch)
        {
        case 1:
            description();
            break;
        case 2:
            show_content();
            break;
        case 3:
            read_file();
            break;
        case 4:
            history(history_list);
            break;
        case 5:
            change_working_dir();
            break;
        case 6:
            get_feedback();
            break;
        default:
            puts("For whom I wrote this instruction...");
            break;
        }

        add_command_history(ch, history_list);
    }

    return 0;
}
