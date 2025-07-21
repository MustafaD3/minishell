NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = minishell.c\
		utils/str.c\
		utils/file.c\
		utils/error.c\
	   	src/environment.c \
		src/execute.c \
		src/builtin/builtin.c \
		src/builtin/env.c \
		src/builtin/cd.c \
		src/builtin/echo.c \
		src/builtin/pwd.c \
		src/builtin/exit.c \
		src/builtin/export.c \
		src/builtin/unset.c \
		src/free.c \
		src/parse/parse_utils.c\
		src/parse/parse.c\
		src/parse/double_quote.c\
		src/parse/single_quote.c\
		src/parse/expand.c\
		src/parse/append.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INC = -I.

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

valgrind:
	make
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)
valgrinderror:
	make
	valgrind -s --track-origins=yes ./$(NAME)

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
