NAME = executor

INC_FLAGS = -I ../../includes -I ../../libft/headers
SRCS = executor.c test_tokens.c


all: $(NAME)

$(NAME):
	gcc  $(SRCS) $(INC_FLAGS) -o $(NAME)

fclean:
	rm $(NAME)

re: fclean all

.PHONY: fclean re all