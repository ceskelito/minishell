include libft/mk.var.export/Makefile

#-------------------#
## BASIC VARIABLES ##
#-------------------#

NAME 		= minishell
CC 			= gcc
CFLAGS 		= -Wall -Wextra -Werror
RM 			= rm -f
MKDIR		= mkdir -p

INC_FLAGS 	= -Iincludes -Ilibft/headers
LIB_FLAGS	= -Llibft -lft -lreadline -lhistory

SRCS_DIR = srcs
OBJS_DIR = objs

FILES = main/main 						\
		tokenizer/ft_strjoin_char	\
		tokenizer/tokenizer			\
		tokenizer/tokenizer_dollar	\
		tokenizer/tokenizer_quotes	\
		tokenizer/tokenizer_word	\
		tokenizer/tokenizer_utils

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

LIBFT = $(LIBFT_ROOT)/$(LIBFT_NAME)

#--------------#
## MAIN RULES ##
#--------------#

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $^ -o $@ $(LIB_FLAGS)

$(OBJS_DIR):
	$(MKDIR) $(OBJS_DIR)

#----------------------#
## EXTERNAL LIBRARIES ##
#----------------------#

$(LIBFT): $(LIBFT_OBJS)
	$(MAKE) -C $(LIBFT_ROOT)

$(LIBFT_OBJS): $(LIBFT_SRCS)

#---------------#
## CLEAN RULES ##
#---------------#

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
