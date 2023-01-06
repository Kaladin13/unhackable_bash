#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MAX_DIGITS 10

#define LOGO_FILENAME "logo.txt"

#define get_char_array_size(array) (sizeof(array) / sizeof(const char *))

int get_int_from_stdin();

void print_menu();

void print_menu_item(int);

void print_logo();

void show_file_content(char *);
