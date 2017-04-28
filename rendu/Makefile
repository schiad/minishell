# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: schiad <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/05 13:10:16 by schiad            #+#    #+#              #
#    Updated: 2017/04/26 12:47:31 by schiad           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= gcc
FLAGS		= -Wall -Wextra -Werror

SRC_DIR		= ./
SRC			= $(SRC_DIR)shell.c\

LIBFT		= ./libft/libft.a
LIBFT_INC	= ./libft/include
LIBFT_DIR	= ./libft

all: $(NAME)

$(NAME): libft
	@echo "Compiling ft_ls project"
	@cd $(SRC_DIR)
	@$(CC) $(FLAGS) $(SRC) -o $(NAME) -I$(LIBFT_INC) \
	-L$(LIBFT_DIR) -lft

libft: $(LIBFT)

$(LIBFT):
	@git submodule update --init --recursive
	@echo "Call LIBFT to compile."
	@make --no-print-directory -C ./libft

fclean:
	@echo "Call LIBFT to fclean."
#	@make fclean --no-print-directory -C ./libft
	@rm -f $(NAME)
	@echo "ft_ls deleted :-(."

clean:
	@echo "Call LIBFT to clean."
	@make clean --no-print-directory -C ./libft

re: fclean all
