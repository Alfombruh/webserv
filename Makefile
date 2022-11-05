NAME = webserv
CC = clang++ 
CFLAGS = -Werror -Wall -Wextra -g3 -fsanitize=address -std=c++98
FILES = srcs/main
SRCS = $(addsuffix .cpp, $(FILES))
OBJS = $(addsuffix .o, $(FILES))


.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -c $^ -o $@

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean: 
	$(RM) *.dSYM
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
