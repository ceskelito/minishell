include libft/mk.var.export/Makefile


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
LIB_FLAGS	:= -Llibft -lft -lreadline -lhistory

#------------------------#
## PROJECT FILES & DIRS ##
#------------------------#

FILES := main 				\
		ft_strjoin_char		\
		tokenizer			\
		tokenizer_dollar	\
		tokenizer_quotes	\
		tokenizer_word		\
		tokenizer_utils		\
		executor			\
		builtin				\
		get_location		\
		

SRCS_DIR := srcs
OBJS_DIR := objs

OBJS_SUB := $(addprefix objs/, $(sort $(dir $(FILES))))

SRCS := $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(FILES)))
OBJS := $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

vpath %.c $(SRC_DIR):$(SRCS_DIR)/executor:$(SRCS_DIR)/parser:$(SRCS_DIR)/tokenizer:$(SRCS_DIR)/main

#----------------------#
## EXTERNAL LIBRARIES ##
#----------------------#

LIBFT := $(LIBFT_ROOT)/$(LIBFT_NAME)

#--------------#
## MAIN RULES ##
#--------------#

all: $(NAME)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR) $(OBJS_SUB)
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

$(LIBFT): $(LIBFT_OBJS)
	$(MAKE) -C $(LIBFT_ROOT)

$(LIBFT_OBJS): $(LIBFT_SRCS)

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

deepre: re
	$(MAKE) re -C $(LIBFT_ROOT)

.PHONY: all clean fclean re deepclean deepfclean deepre
