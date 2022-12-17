NAME = webserv
CC = clang++ 
CFLAGS =-Werror -Wall -Wextra -std=c++98 -g3 -fsanitize=address
FILES = main 						\
		srcs/config/configParser	\
		srcs/errors					\
		srcs/classes/config			\
		srcs/classes/Server			\
		srcs/classes/Request		\
		srcs/classes/Response		\
		srcs/classes/Router			\
		routes/index				\
		routes/cgi					\
		routes/login				\
		routes/galery				\
		routes/public				\

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

test:
	./testers/tester http://localhost:8080

clean:
	$(RM) *.dSYM
	$(RM) *.DS_Store
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
