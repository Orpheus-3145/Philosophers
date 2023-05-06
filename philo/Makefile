# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fra <fra@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/23 14:01:32 by fra               #+#    #+#              #
#    Updated: 2023/05/06 15:44:41 by fra              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/bash

NAME := philo
SRC_DIR := sources
OBJ_DIR := objects
INCLUDE  := include

HEADERS := $(shell find include -type f -name '*.h')
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SOURCES:.c=.o))

CC  := gcc
IFLAGS := -Iinclude
CFLAGS := -Wall -Wextra -Werror -O3 #-g3 -fsanitize=thread

GREEN = \x1b[32;01m
RED = \x1b[31;01m
BLUE = \x1b[34;01m
RESET = \x1b[0m
YELLOW = \x1b[33;01m

all: $(NAME)	

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS) -o $(NAME)
	@TSAN_OPTIONS=second_deadlock_stack=1
	@printf "(philosophers) $(GREEN)Created program $(NAME)$(RESET)\n"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@TSAN_OPTIONS=second_deadlock_stack=1
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@printf "(philosophers) $(BLUE)Created object $$(basename $@)$(RESET)\n"

clean:
	@for file in $(shell find $(OBJ_DIR) -type f -name '*.o'); do 	\
		rm -f $$file;	\
		printf "(philosophers) $(RED)Removed object $$(basename $$file)$(RESET)\n"; \
	done

fclean: clean
	@-rm -f $(NAME)
	@printf "(philosophers) $(RED)Removed program $(NAME)$(RESET)\n"

re: fclean all

.PHONY: all, clean, fclean, re