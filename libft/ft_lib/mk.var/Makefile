FTLIB_OBJS_DIR = objs

FTLIB_IS 	= ft_isalnum.c ft_isalpha.c ft_isascii.c	\
		ft_isdigit.c ft_isprint.c ft_isspace.c

FTLIB_LST 	= ft_lstadd_back.c ft_lstclear.c ft_lstiter.c	\
		ft_lstmap.c ft_lstsize.c ft_lstadd_front.c		\
		ft_lstdelone.c ft_lstlast.c ft_lstnew.c

FTLIB_MEM 	= ft_bzero.c ft_memchr.c ft_memcpy.c	\
		ft_memset.c ft_calloc.c ft_memcmp.c		\
		ft_memmove.c
		
FTLIB_PUT 	= ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c
		
FTLIB_STR 	= ft_putstr_fd.c ft_strdup.c ft_strlcat.c 	\
		ft_strmapi.c ft_strrchr.c ft_split.c 		\
		ft_striteri.c ft_strlcpy.c ft_strncmp.c 	\
		ft_strtrim.c ft_strchr.c ft_strjoin.c 		\
		ft_strlen.c ft_strnstr.c ft_substr.c		\
		ft_strcmp.c ft_strjoin_multi.c

FTLIB_TO 	= ft_atoi.c ft_itoa.c ft_tolower.c ft_toupper.c ft_atol.c

FTLIB_SRCS	= $(addprefix ft_is/, $(FTLIB_IS)) $(addprefix ft_lst/, $(FTLIB_LST)) \
		$(addprefix ft_mem/, $(FTLIB_MEM)) $(addprefix ft_put/, $(FTLIB_PUT)) \
		$(addprefix ft_str/, $(FTLIB_STR)) $(addprefix ft_to/, $(FTLIB_TO))

FTLIB_OBJS = $(addprefix $(FTLIB_OBJS_DIR)/, $(notdir $(FTLIB_SRCS:.c=.o)))