MAKE_PATH	:= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
LIBFT_ROOT	:= $(shell dirname $(MAKE_PATH))

include $(LIBFT_ROOT)/ft_lib/mk.var/Makefile
include $(LIBFT_ROOT)/ft_printf/mk.var/Makefile
include $(LIBFT_ROOT)/get_next_line/mk.var/Makefile

FT_LIB_SRCS		= $(addprefix $(LIBFT_ROOT)/ft_lib/, $(FTLIB_SRCS))
FT_LIB_OBJS		= $(addprefix $(LIBFT_ROOT)/ft_lib/, $(FTLIB_OBJS))

FT_PRINTF_SRCS	= $(addprefix $(LIBFT_ROOT)/ft_printf/, $(PF_SRCS))
FT_PRINTF_OBJS	= $(addprefix $(LIBFT_ROOT)/ft_printf/,	$(PF_OBJS))

FT_GNL_SRCS		= $(addprefix $(LIBFT_ROOT)/get_next_line/,	$(GNL_SRCS))
FT_GNL_OBJS		= $(addprefix $(LIBFT_ROOT)/get_next_line/,	$(GNL_OBJS))

LIBFT_NAME		= libft.a
LIBFT_SRCS		= $(FT_LIB_SRCS) $(FT_PRINTF_SRCS) $(FT_GNL_SRCS)
LIBFT_OBJS		= $(FT_LIB_OBJS) $(FT_PRINTF_OBJS) $(FT_GNL_OBJS)
