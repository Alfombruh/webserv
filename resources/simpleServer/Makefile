SNAME = server
CNAME = client
CC = clang++
CFLAGS = -Werror -Wall -Wextra -g3 -fsanitize=address
SFILES = server
CFILES = client
SSRCS = $(addsuffix .cpp,$(SFILES))
SOBJS = $(addsuffix .o,$(SFILES))
CSRCS = $(addsuffix .cpp,$(CFILES))
COBJS = $(addsuffix .o,$(CFILES))

.cpp.o: $(SSRCS) $(CSRCS)
	$(CC) $(CFLAGS) -c $(SSRCS) -o $(SOBJS)
	$(CC) $(CFLAGS) -c $(CSRCS) -o $(COBJS)

all: $(SOBJS) $(COBJS)
	$(CC) $(CFLAGS) $(SOBJS) -o $(SNAME)
	$(CC) $(CFLAGS) $(COBJS) -o $(CNAME)

clean:
	rm -rf $(SOBJS) $(COBJS) *.dSYM

fclean: clean
	rm -rf $(SNAME) $(CNAME)

re: fclean all

.PHONY: all clean fclean re