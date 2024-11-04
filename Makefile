NAME = philo

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[38;5;229m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
LIGHT_CYAN = \033[1;36m
CYAN = \033[0;36m
DARK_CYAN = \033[2;36m
WHITE = \033[0;37m
RESET = \033[0m

SRCS =	philo.c \
		init.c \
		utils.c

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror
#LOADING_MSG = "$(CYAN)Loading libft...$(RESET)"
LOADING_COMPLETE = \
					"		\n"\
"$(RED)        _     _ _                       _$(RESET)\n"\
"$(RED)       |*|   (_)*|                     |*|$(RESET)\n"\
"$(RED)  _ __ |*|__  _|*| ___  ___  ___  _ __ |*|__   ___ _ __ ___ $(RESET)\n"\
"$(RED) |*'_*\|*'_*\|*|*|/*_*\/*__|/*_*\|*'_*\|*'_*\ /*_*\*'__/*__| $(RESET)\n"\
"$(RED) |*|_)*|*| |*|*|*|*(_)*\__*\*(_)*|*|_)*|*| |*|**__/*|  \__*\ $(RESET)\n"\
"$(RED) |*.__/|_| |_|_|_|\___/|___/\___/|*.__/|_| |_|\___|_|  |___/ $(RESET)\n"\
"$(RED) |*|                             |*|$(RESET)\n"\
"$(RED) |_|                             |_|                        $(RESET)\n"\


all: message $(NAME)

message:
	@printf "$(RED)Loading philo...  "
#@spin='-\|/'; \
#i=0; \
#while [ $$i -lt 12 ]; do \
#	printf "\b$$(expr $$i % 4)"; \
#	printf "\b$${spin:$$((i % 4)):1}"; \
#	sleep 0.25; \
#	i=$$((i + 1)); \
#done
	@printf "$(RESET)\n"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@chmod +x $(NAME)
	@echo $(LOADING_COMPLETE)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean message $(NAME)


.PHONY : all clean fclean re bonus
