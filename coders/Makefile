# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/29 14:26:36 by tlaranje          #+#    #+#              #
#    Updated: 2026/01/30 13:18:27 by tlaranje         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === Executable name ===
NAME	:= codexion

# === Flags ===
CFLAGS	:= -Wall -Wextra -Werror

# === Source files ===
SRC		:= main.c check_args.c

# === Object files ===
OBJ_DIR	:= objs
OBJ		:= $(SRC:%.c=$(OBJ_DIR)/%.o)

# === Build targets ===
all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	cc $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re