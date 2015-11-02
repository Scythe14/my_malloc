CC	= gcc

RM	= rm -f

MACHINE	= $(shell uname -m)

CFLAGS	= -Wall -Wextra -Werror

NAME	= libmy_malloc_$(MACHINE).so

SYMBOL	= libmy_malloc.so

SRC	= malloc.c		\
	  free.c		\
	  realloc.c		\
	  calloc.c		\
	  show_alloc_mem.c	\
	  malloc_split.c	\
	  free_error.c

OBJ	= $(SRC:.c=.o)

all:	$(NAME)

$(NAME): $(OBJ)
	$(CC) -fPIC -c $(SRC)
	$(CC) -shared $(OBJ) -o $(NAME)
	ln --symbolic -f $(NAME) $(SYMBOL)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME) $(SYMBOL)

re: fclean all

.PHONY: all clean fclean re
