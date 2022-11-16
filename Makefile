NAME = webserv
CC = clang++ 
CFLAGS = -Werror -Wall -Wextra -g3 -fsanitize=address -std=c++98 
FILES = main
SRCS = $(addsuffix .cpp, $(FILES))
OBJS = $(addsuffix .o, $(FILES))
INCLUDES = includes/Response.hpp

#.SILENT:

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $^ -o $@

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INCLUDES) $(OBJS) -o $@

run: $(NAME)
	./webserv

clean: 
	$(RM) *.dSYM
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
