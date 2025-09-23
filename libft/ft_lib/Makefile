include ../mk.var.local/Makefile
include mk.var/Makefile

NAME 		= libftlib.a

OBJS_DIR	= $(FTLIB_OBJS_DIR)
OBJS		= $(FTLIB_OBJS)

# Setting the sources path
vpath %.c $(sort $(dir $(FTLIB_SRCS)))

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

.PHONY: all objects clean fclean re
