#pragma once

typedef struct command_history
{
    int command;
    struct command_history *next_command;
} command_history;

void add_command_history(int, command_history *);

void list_command_history(command_history *);