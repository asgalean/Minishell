# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/23 12:45:00 by cvon-hee          #+#    #+#              #
#    Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### Hide terminal output ###
.SILENT:

### Project Info ###
NAME        = minishell

### Compiler ###
CC          = clang
CFLAGS      = -Wall -Wextra -Werror -g -I includes/ -I libft/includes/
RM          = rm -rf

### Colors ###
GREEN       = "\033[32m"
RED         = "\033[31m"
YELLOW      = "\033[33m"
RESET       = "\033[0m"

### Paths ###
INC_DIR     = includes
LIBFT_PATH  = libft

### Libs ###
LIBFT_LIB   = -L $(LIBFT_PATH) -lft

### Header ###
HEADER      = minishell.h

### Source groups ###
BUILTINS    = cd echo env exit export pwd unset is_builtin
EXEC        = exec_tree resolve_path exec_builtin exec_pipe exec_redir \
exec_simple exec_heredoc handle_redis heredoc_utils
MAIN        = main signals shell_setup prompt
ENV         = env_utils status env_set lvl_shell
PARSER      = tokenizer expand_var command_tree read_double read_unquoted \
unquoted_utils parse_redirs parse_exec
UTILS       = error memory wait built_ins print utils free_tree

### Source files ###
SRC =  $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
    $(addsuffix .c, $(addprefix srcs/exec/, $(EXEC))) \
    $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
    $(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
    $(addsuffix .c, $(addprefix srcs/utils/, $(UTILS))) \
    $(addsuffix .c, $(addprefix srcs/parser/, $(PARSER)))

### Object files ###
OBJ = $(SRC:.c=.o)

### Rules ###
all: $(NAME)

$(NAME): $(OBJ)
	@clear
	@echo "\033[32m"
	@echo " ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗    "
	@echo " ████╗ ████║██║████╗  ██║██╝██╔════╝██║  ██║██╔════╝██║     ██║    "
	@echo " ██╔████╔██║██║██╔██╗ ██║██╗███████╗███████║█████╗  ██║     ██║    "
	@echo " ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║    "
	@echo " ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@echo " ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "                        ____      _      __   __             "
	@echo "      _______________ _/ / / ____(_)__ _/ /  / /_____________ "
	@echo "     /___/___/___/ _ \`/ / / / __/ / _ \`/ _ \/ __/___/___/___/ "
	@echo "                 \\_,_/_/_/ /_/ /_/\\_, /_//_/\\__/             "
	@echo "                                 /___/                        "
	@echo $(RESET)
	@make -s -C $(LIBFT_PATH)
	@echo $(YELLOW)"Compiling minishell..."$(RESET)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_LIB) -lreadline
	@echo $(GREEN)"MINISHELL successfully compiled!"$(RESET)
	@echo $(YELLOW)"Exec: ./minishell"$(RESET)

### Object compilation ###
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

### Cleaning ###
clean:
	@echo $(RED)"Cleaning object files..."$(RESET)
	@$(RM) $(OBJ)
	@make -s -C $(LIBFT_PATH) clean

fclean: clean
	@echo $(RED)"Removing executable..."$(RESET)
	@$(RM) $(NAME)
	@make -s -C $(LIBFT_PATH) fclean
	@echo $(RED)"MINISHELL successfully cleaned!"$(RESET)

re: fclean all

### Extra ###
test: all
	./$(NAME)

norm:
	norminette $(SRC) $(INC_DIR)/$(HEADER)

.PHONY: all clean fclean re test norm
