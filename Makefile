# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luialvar <luialvar@student.42malaga.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/08 15:50:01 by luialvar          #+#    #+#              #
#    Updated: 2024/12/08 15:50:10 by luialvar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf

SRCS = printing.c \
       main.c \
       create_matrix.c \
       src/mlx_init.c \
       external_functions.c \
       ft_split.c \
       get_next_line/get_next_line.c \
       get_next_line/get_next_line_utils.c

LIBS = libmlx42.a

INCLUDES = -I./src -I./get_next_line

OBJ = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lglfw -lGL -lm

all: $(NAME)

$(NAME): $(OBJ) $(LIBS)
	$(CC) $(OBJ) $(LIBS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: re all clean fclean
