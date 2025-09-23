NAME 		= minishell
CC 			= gcc
CFLAGS 		= -Wall -Wextra -Werror
INC_FLAGS 	= -Iincludes
RM 			= rm -f
MKDIR		= mkdir -p

SRCS_DIR = srcs
OBJS_DIR = objs

FILES = tokenizer \
		tokenizer_utils

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

all: $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(NAME): $(OBJS) 
	$(CC) $^ -o $@

$(OBJS_DIR):
	$(MKDIR) $(OBJS_DIR)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
