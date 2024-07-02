CC = 			cc

CFLAGS = 		-Wall -Wextra -Werror

RM = 			rm -f

NAME = 			pipex

SRCS =			srcs/pipex.c \
				srcs/pipex_utils.c \
				srcs/get_next_line.c \
				srcs/get_next_line_utils.c \
				srcs/ft_split.c \


BANNER = "\n\n\e[0;37m ██████╗ ██╗██████╗ ███████╗██╗  ██╗  \n\
██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝  \n\
██████╔╝██║██████╔╝█████╗   ╚███╔╝   \n\
██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗   \n\
██║     ██║██║     ███████╗██╔╝ ██╗  \n\
╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝  \n\
								by febouana\n"		

all: 			$(NAME)

$(NAME):
	@$(CC) $(SRCS) $(CFLAGS) -o $(NAME)
	@echo -e $(BANNER)

re: 		fclean all	
	@echo "\e[0;37m[✅ restored]"

clean:
	@echo "\e[0;37m[🧹...cleaning]"
	@$(RM) $(NAME)

fclean: 	clean
	@$(RM) $(NAME)

.PHONY: 	fclean clean re all
