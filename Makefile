CC = 			cc

CFLAGS = 		-Wall -Wextra -Werror

RM = 			rm -f

NAME = 			pipex

SRC_M =			mandatory/pipex.c \
				mandatory/pipex_utils.c \
				mandatory/gestion_error.c \
				mandatory/utils/get_next_line.c \
				mandatory/utils/get_next_line_utils.c \
				mandatory/utils/ft_split.c \

SRC_B =			bonus/pipex_bonus.c \
				bonus/pipex_utils_bonus.c \
				bonus/heredoc_bonus.c \
				bonus/gestion_error_bonus.c \
				bonus/utils/get_next_line.c \
				bonus/utils/get_next_line_utils.c \
				bonus/utils/ft_split.c \


BANNER_M = "\n\n\e[0;37m ██████╗ ██╗██████╗ ███████╗██╗  ██╗  \n\
██╔══██╗██║██╔══██╗██╔════╝╚██╗██╔╝  \n\
██████╔╝██║██████╔╝█████╗   ╚███╔╝   \n\
██╔═══╝ ██║██╔═══╝ ██╔══╝   ██╔██╗   \n\
██║     ██║██║     ███████╗██╔╝ ██╗  \n\
╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝  \n\
								by febouana\n"		

BANNER_B = "\n\n\e[0;37m ██████╗  ██████╗ ███╗   ██╗██╗   ██╗███████╗  \n\
██╔══██╗██╔═══██╗████╗  ██║██║   ██║██╔════╝  \n\
██████╔╝██║   ██║██╔██╗ ██║██║   ██║███████╗  \n\
██╔══██╗██║   ██║██║╚██╗██║██║   ██║╚════██║  \n\
██████╔╝╚██████╔╝██║ ╚████║╚██████╔╝███████║  \n\
╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝  \n\
								by febouana\n"		

all: 			$(NAME)

$(NAME):
	@$(CC) $(SRC_M) $(CFLAGS) -o $(NAME)
	@echo -e $(BANNER_M)

re: 		fclean all	
	@echo "\e[0;37m[✅ restored]"

clean:
	@echo "\e[0;37m[🧹...cleaning]"
	@$(RM) $(NAME)

fclean: 	clean
	@$(RM) $(NAME)

b:	clean 
	@$(CC) $(SRC_B) $(CFLAGS) -o $(NAME)
	@echo -e $(BANNER_B)

.PHONY: 	fclean clean re all
