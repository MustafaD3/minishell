NAME = libft.a

FUNCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
		ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_split.c ft_strchr.c \
		ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
		ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
		ft_strndup.c ft_isspace.c ft_strjoinchr.c ft_isquote.c ft_isredir.c ft_ismeta.c ft_strcmp.c\
		ft_strstr.c

BFUNCS = ft_lstnew.c ft_lstadd_front.c ft_lstadd_back.c ft_lstsize.c \
		 ft_lstlast.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

OBJECTS = $(FUNCS:.c=.o)
BONUS_OBJECTS = $(BFUNCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror
REM = rm -f
AR = ar rcs

all: $(NAME)

$(NAME): $(OBJECTS) $(BONUS_OBJECTS)
	$(AR) $(NAME) $(OBJECTS) $(BONUS_OBJECTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	$(REM) $(OBJECTS) $(BONUS_OBJECTS)

fclean: clean
	$(REM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
