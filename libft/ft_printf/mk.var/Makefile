PF_OBJS_DIR = objs

PF_PRINT_SRCS = ft_printchr.c ft_printhex.c ft_printnbr.c	\
			ft_printptr.c ft_printstr.c ft_printunbr.c

PF_UTILS_SRCS = ft_count_digit_base.c ft_putnbr_fd.c	\
			ft_putnbr_hex.c ft_putunbr.c	\
			ft_putptr.c 

PF_SRCS	= 	ft_printf.c \
			$(addprefix print/, $(PF_PRINT_SRCS)) \
			$(addprefix utils/, $(PF_UTILS_SRCS))

PF_OBJS = $(addprefix $(PF_OBJS_DIR)/, $(notdir $(PF_SRCS:.c=.o)))