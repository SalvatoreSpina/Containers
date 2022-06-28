NAME =		ft_containers

COMP =		c++
CFLAGS =	-Wall -Werror -Wextra -std=c++98

SRCS =		main.cpp

OBJS =		$(SRCS:%.cpp=%.o)

$(NAME):	$(SRCS)
	$(COMP) $(CFLAGS) $(SRCS) -o $(NAME)

all: $(NAME)

clean:
	@		$(RM) $(OBJS)
	@		rm -rf containers_test/

fclean:		clean
	@		$(RM) $(NAME)
	@		rm -rf *.out

re:			fclean $(NAME)

tester:
	@./scripts/clone.sh

test:
	@./scripts/test.sh

speedtest:
	@./scripts/do.sh

leak:
	@./scripts/leaks.sh