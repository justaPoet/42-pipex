CC = 			cc

CFLAGS = 		-Wall -Wextra -Werror

RM = 			rm -f

NAME = 			pipex

SRC =			src/pipex.c \
				src/pipex_utils.c \
				src/get_next_line.c \
				src/get_next_line_utils.c \
				src/ft_split.c \
				src/gestion_error.c \


BANNER = "\n\n\e[0;37m ██████╗ ██╗██████╗ ███████╗██╗  ██╗  \n\
██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝  \n\
██████╔╝██║██████╔╝█████╗   ╚███╔╝   \n\
██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗   \n\
██║     ██║██║     ███████╗██╔╝ ██╗  \n\
╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝  \n\
								by febouana\n"		

all: 			$(NAME)

$(NAME):
	@$(CC) $(SRC) $(CFLAGS) -o $(NAME)
	@echo $(BANNER)

re: 		fclean all	
	@echo "\e[0;37m[✅ restored]"

clean:
	@echo "\e[0;37m[🧹...cleaning]"
	@$(RM) $(NAME)

fclean: 	clean
	@$(RM) $(NAME)

.PHONY: 	fclean clean re all
