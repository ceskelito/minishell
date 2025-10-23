include libft/mk.var.export/Makefile

#─────────────#
## FUNCTIONS ##
#─────────────#
define write_flags
	echo "$1" | tr ' ' '\n' | sed -E 's/^(-[A-Za-z])/\1\n/' >> $2
endef
	
#──────────────────#
## PROJECT CONFIG ##
#──────────────────#

NAME 		:= minishell
CC 			:= gcc
CFLAGS 		:= -Wall -Wextra -Werror
RM 			:= rm -f
MKDIR		:= mkdir -p
INC_FLAGS 	:= -Iincludes -Ilibft/headers -Iezalloc/include
LIB_FLAGS	:= -Llibft -lft -lreadline -Lezalloc -lezalloc
			
#────────────────────────#
## PROJECT FILES & DIRS ##
#────────────────────────#

FILES = main 				\
		ft_strjoin_char		\
		tokenizer			\
		tokenizer_dollar	\
		tokenizer_quotes	\
		tokenizer_word		\
		tokenizer_utils		\
		tokenizer_debug		\
		parser				\
		parser_utils		\
		parser_cleanup		\
		parser_debug		\
		shell_init			\
		executor			\
		builtin				\
		get_location		\
		heredoc

SRCS_DIR := srcs
OBJS_DIR := objs

vpath %.c	$(SRCS_DIR) \
			:$(SRCS_DIR)/main \
			:$(SRCS_DIR)/tokenizer \
			:$(SRCS_DIR)/parser \
			:$(SRCS_DIR)/executor

SRCS := $(addsuffix .c, $(FILES))
OBJS := $(addsuffix .o, $(addprefix $(OBJS_DIR)/, $(FILES)))

#──────────────────────#
## EXTERNAL LIBRARIES ##
#──────────────────────#

LIBFT 		= $(LIBFT_ROOT)/$(LIBFT_NAME)

LIBEZ_ROOT	= ezalloc
LIBEZ_NAME	= libezalloc.a
LIBEZ		= $(LIBEZ_ROOT)/$(LIBEZ_NAME)

LIBRARIES 	= $(LIBFT) $(LIBEZ)

#──────────────#
## MAIN RULES ##
#──────────────#

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< $(INC_FLAGS) -o $@

all: $(NAME)

$(NAME): $(OBJS) $(LIBRARIES)
	$(CC) $^ $(LIB_FLAGS) -o $@ 

$(OBJS_DIR):
	$(MKDIR) $@

#─────────────#
## LIBRARIES ##
#─────────────#

$(LIBFT): $(LIBFT_OBJS)
$(LIBFT_OBJS): $(LIBFT_SRCS)
	$(MAKE) -C $(LIBFT_ROOT)

$(LIBEZ):
	$(MAKE) -C $(LIBEZ_ROOT)

#─────────#
## TOOLS ##
#─────────#

compile_flags.txt: Makefile
	$(RM) $@
	$(call write_flags,$(INC_FLAGS),$@)
	$(call write_flags,$(LIB_FLAGS),$@)

#───────────────#
## CLEAN RULES ##
#───────────────#

clean:
	$(RM) -r $(OBJS_DIR)
	@echo "Call 'make deep$@' to propagate the command to the external libraries\n"

fclean: clean
	$(RM) $(NAME)
	@echo "Call 'make deep$@' to propagate the command to the external libraries\n"

re: fclean all
	@echo "Call 'make deep$@' to propagate the command to the external libraries\n"

deepclean: clean
	$(MAKE) clean -C $(LIBFT_ROOT)
	$(MAKE) clean -C $(LIBEZ_ROOT)

deepfclean: fclean
	$(MAKE) fclean -C $(LIBFT_ROOT)
	$(MAKE) fclean -C $(LIBEZ_ROOT)

deepre: deepfclean all

.PHONY: all clean fclean re deepclean deepfclean deepre
