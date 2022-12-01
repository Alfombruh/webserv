NAME = webserv
CC = clang++ 
CFLAGS = -Werror -Wall -Wextra -g3 -fsanitize=address -std=c++98 
FILES = main 					\
		srcs/errors				\
		srcs/classes/Server		\
		srcs/classes/Request	\
		srcs/classes/Response	\
		srcs/classes/Router		\
		routes/index			\
		routes/upload			\
		routes/public			\

SRCS = $(addsuffix .cpp, $(FILES))
OBJS = $(addsuffix .o, $(FILES))

INCLUDES =	includes/ 


#.SILENT:

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $^ -o $@

all: $(NAME)

$(NAME): $(OBJS) $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INCLUDES) $(OBJS) -o $@

run: $(NAME)
	./webserv config/default.conf

clean: 
	$(RM) *.dSYM
	$(RM) *.DS_Store
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
