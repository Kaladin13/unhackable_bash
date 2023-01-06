# Name of the challenge executable to build
TARGET := unhackable_bash

# Architecture
BITS := 64

SRCS := main.c src/utils.c src/custom_shell.c src/command_history.c

# Forbidden code execution from stack (-z execstack)
NX := 1

# Address space layout randomization enabled
ASLR := 1

# Full relro
RELRO := 1

# Enable stack canaries
CANARY := 1

DOCKER_PORTS := 20023
DOCKER_IMAGE := unhackable_bash_image

PUBLISH_BUILD := $(TARGET)
PUBLISH := logo.txt
PUBLISH_LIBC := docker-libc.so