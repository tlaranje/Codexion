# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/29 14:26:36 by tlaranje          #+#    #+#              #
#    Updated: 2026/02/11 17:14:35 by tlaranje         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === Executable name ===
NAME	:= codexion

# === Flags ===
CFLAGS	:= -Wall -Wextra -Werror -g -Iincludes #-fsanitize=thread

# === Source files ===
SRC_DIR	:= srcs
SRC		:= $(shell find $(SRC_DIR) -name "*.c")

# === Object files ===
OBJ_DIR	:= objs
OBJ		:= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# === Build targets ===
all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	cc $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm $(NAME)

re: clean all

.PHONY: all clean fclean re
