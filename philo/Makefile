NAME = philo

CC = cc
CFLAGS = -Wall -Werror -Wextra -g -pthread -Wno-deprecated-declarations

SRCS =	src/main.c \
		src/utils.c \
		src/init.c \
		src/philo_routin.c \
		src/philo_routin_utils.c \
		src/monitor.c

OBJS = $(SRCS:.c=.o)
%.o :	%.c philo.h
		$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) :	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean :
		rm -rf $(OBJS)

fclean :
		rm -rf $(NAME)
		rm -rf $(OBJS)
re :	fclean all