NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRCS = minishell.c\
		utils/str.c\
		utils/file.c\
		utils/error.c\
	   	src/environment.c \
		src/display_info.c \
		src/executor/execute.c \
		src/executor/execute_utils.c \
		src/executor/execute_builtins.c \
		src/executor/execute_pipe_helpers.c \
		src/executor/execute_helpers.c \
		src/redirection/redirection_core.c \
		src/redirection/redirection_parser.c \
		src/redirection/redirection_setup.c \
		src/redirection/redirection_process.c \
		src/redirection/redirection_apply.c \
		src/redirection/redirection_helpers.c \
		src/builtin/builtin.c \
		src/builtin/env.c \
		src/builtin/cd.c \
		src/builtin/echo.c \
		src/builtin/pwd.c \
		src/builtin/exit.c \
		src/builtin/export.c \
		src/builtin/export_utils.c \
		src/builtin/unset.c \
		src/free/free.c \
		src/free/free_two.c \
		src/free/free_three.c \
		src/utils/command_preprocessor.c \
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

leak: 
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=valgrind.supp --track-origins=yes ./$(NAME)
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
