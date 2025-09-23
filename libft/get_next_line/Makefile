include ../mk.var.local/Makefile
include mk.var/Makefile

NAME		= libgnl.a
OBJS_DIR	= $(GNL_OBJS_DIR)
OBJS		= $(GNL_OBJS)

vpath %.c .

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN)Compiling $(BLUE)$<$(RESET)\n"

$(OBJS_DIR):
	$(MKDIR) $(OBJS_DIR)

$(NAME): $(OBJS)
	$(AR) $@ $^

all: $(NAME)

objects: $(OBJS)

clean:
	@$(RM) $(OBJS)
	@printf "$(RED)Cleaning objects of $(BLUE)$(NAME)$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@$(RM) -r $(OBJS_DIR)
	@printf "$(RED)Deliting $(BLUE)$(NAME)$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re objects
