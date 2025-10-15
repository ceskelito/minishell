# Убрал проблемную строку: include libft/mk.var.local/Makefile

#-------------#
## FUNCTIONS ##
#-------------#
define write_flags
	echo "$1" | tr ' ' '\n' | sed -E 's/^(-[A-Za-z])/\1\n/' >> $2
endef
	
#-------------------------#
## COMMAND & COMPILATION ##
#-------------------------#

NAME 		:= minishell
CC 			:= gcc
CFLAGS 		:= -Wall -Wextra -Werror
RM 			:= rm -f
MKDIR		:= mkdir -p

INC_FLAGS 	:= -Iincludes -Ilibft/headers
LIB_FLAGS	:= -Llibft -lft -lreadline

#------------------------#
## PROJECT FILES & DIRS ##
#------------------------#

FILES := main/main 					\
		tokenizer/ft_strjoin_char	\
		tokenizer/tokenizer			\
		tokenizer/tokenizer_dollar	\
		tokenizer/tokenizer_quotes	\
		tokenizer/tokenizer_word	\
		tokenizer/tokenizer_utils	\
		tokenizer/tokenizer_debug	\
									\
		parser/parser				\
		parser/parser_utils			\
		parser/parser_cleanup		\
		parser/parser_debug			\
		parser/shell_init

SRCS_DIR := srcs
OBJS_DIR := objs

OBJS_SUB := $(addprefix objs/, $(sort $(dir $(FILES))))

SRCS := $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))
OBJS := $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

#----------------------#
## EXTERNAL LIBRARIES ##
#----------------------#

LIBFT_ROOT := libft
LIBFT_NAME := libft.a

LIBFT := $(LIBFT_ROOT)/$(LIBFT_NAME)

#--------------#
## MAIN RULES ##
#--------------#

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR) $(OBJS_SUB)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS) -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $^ $(LIB_FLAGS) -o $@ 

#---------------#
## DIRECTORIES ##
#---------------#

$(OBJS_DIR):
$(OBJS_SUB): $(OBJS_DIR)
	$(MKDIR) $@

#----------------------#
## EXTERNAL LIBRARIES ##
#----------------------#

$(LIBFT):
	$(MAKE) -C $(LIBFT_ROOT)

#---------#
## TOOLS ##
#---------#

compile_flags.txt: Makefile
	$(RM) $@
	$(call write_flags,$(INC_FLAGS),$@)
	$(call write_flags,$(LIB_FLAGS),$@)

#---------------#
## CLEAN RULES ##
#---------------#

clean:
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

deepclean: clean
	$(MAKE) clean -C $(LIBFT_ROOT)

deepfclean: fclean
	$(MAKE) fclean -C $(LIBFT_ROOT)

deepre: deepfclean all

.PHONY: all clean fclean re deepclean deepfclean deepre
