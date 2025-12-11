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
CFLAGS 		:= -Wall -Wextra -Werror -g
RM 			:= rm -f
MKDIR		:= mkdir -p
READLINE_DIR := $(shell brew --prefix readline)

INC_FLAGS 	:= -Iincludes -Ilibft/headers -Iezalloc/include \
			   -I$(READLINE_DIR)/include

LIB_FLAGS	:= -Llibft -lft -Lezalloc -lezalloc \
			   -L$(READLINE_DIR)/lib -lreadline


			
#────────────────────────#
## PROJECT FILES & DIRS ##
#────────────────────────#

EXECUTOR = 	echo-cd-pwd-exit		\
			env-unset				\
			export					\
			executor				\
			heredoc					\
			pipe					\
			redirections			\
			resolve_command_location

TOKENIZER = ft_strjoin_char			\
			tokenizer				\
			tokenizer_concatenate	\
			tokenizer_debug			\
			tokenizer_dollar		\
			tokenizer_quotes		\
			tokenizer_utils			\
			tokenizer_word          \
			tokenizer_word_utils

PARSER =	parser					\
			parser_cmd_utils		\
			parser_cmd_handlers		\
			parser_redirs			\
			parser_cleanup			\
			parser_debug

UTILS = 	array					\
			cleanup					\
			exit_status 			\
			general_utils 			\
			get_prompt				\
			init 					\
			signals					\
			split_in_chunks 		\
			string_expand_dollars

ENVIRON = 	environment 			\
			environment_handler

FILES = main 			\
		$(TOKENIZER) 	\
		$(PARSER) 		\
		$(EXECUTOR) 	\
		$(UTILS)		\
		$(ENVIRON)

SRCS_DIR := srcs
OBJS_DIR := objs

vpath %.c	$(SRCS_DIR) \
			:$(SRCS_DIR)/tokenizer \
			:$(SRCS_DIR)/parser \
			:$(SRCS_DIR)/executor \
			:$(SRCS_DIR)/executor/builtins \
			:$(SRCS_DIR)/utils \
			:$(SRCS_DIR)/environment

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
	@$(CC) $(CFLAGS) -c $< $(INC_FLAGS) -o $@
	@echo "Compiling $<"

all: $(NAME) last_print

$(NAME): $(OBJS) $(LIBRARIES)
	@$(CC) $^ $(LIB_FLAGS) -o $@ 
	@echo "Linkng Objects"

$(OBJS_DIR):
	$(MKDIR) $@

#─────────────#
## LIBRARIES ##
#─────────────#

$(LIBFT):
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

last_print:
	@echo "▖  ▖  ▘      ▘  ▄▖  ▌       ▜   ▜     ";
	@echo "▛▖▞▌  ▌  ▛▌  ▌  ▚   ▛▌  █▌  ▐   ▐     ";
	@echo "▌▝ ▌  ▌  ▌▌  ▌  ▄▌  ▌▌  ▙▖  ▐▖  ▐▖    ";
	@echo "                                      ";
	@echo "                 ▘▜    ▌              ";
	@echo "        ▛▘▛▌▛▛▌▛▌▌▐ █▌▛▌              ";
	@echo "        ▙▖▙▌▌▌▌▙▌▌▐▖▙▖▙▌              ";
	@echo "               ▌                      ";

run-test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp  ./$(NAME)


# last_print:
# 	@echo "███╗   ███╗    ██╗    ███╗   ██╗    ██╗    ███████╗    ██╗  ██╗    ███████╗    ██╗         ██╗             ";
# 	@echo "████╗ ████║    ██║    ████╗  ██║    ██║    ██╔════╝    ██║  ██║    ██╔════╝    ██║         ██║             ";
# 	@echo "██╔████╔██║    ██║    ██╔██╗ ██║    ██║    ███████╗    ███████║    █████╗      ██║         ██║             ";
# 	@echo "██║╚██╔╝██║    ██║    ██║╚██╗██║    ██║    ╚════██║    ██╔══██║    ██╔══╝      ██║         ██║             ";
# 	@echo "██║ ╚═╝ ██║    ██║    ██║ ╚████║    ██║    ███████║    ██║  ██║    ███████╗    ███████╗    ███████╗        ";
# 	@echo "╚═╝     ╚═╝    ╚═╝    ╚═╝  ╚═══╝    ╚═╝    ╚══════╝    ╚═╝  ╚═╝    ╚══════╝    ╚══════╝    ╚══════╝        ";
# 	@echo "                                                                                                           ";
# 	@echo "                 ██████╗ ██████╗ ███╗   ███╗██████╗ ██╗██╗     ███████╗██████╗                             ";
# 	@echo "                ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██║██║     ██╔════╝██╔══██╗                            ";
# 	@echo "                ██║     ██║   ██║██╔████╔██║██████╔╝██║██║     █████╗  ██║  ██║                            ";
# 	@echo "                ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██║██║     ██╔══╝  ██║  ██║                            ";
# 	@echo "                ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ██║███████╗███████╗██████╔╝                            ";
# 	@echo "                 ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚═╝╚══════╝╚══════╝╚═════╝                             ";
#	@echo "                                                                                                           ";
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

.PHONY: all clean fclean re deepclean deepfclean deepre last_print
