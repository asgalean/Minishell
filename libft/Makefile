# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asgalean <asgalean@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/23 12:08:32 by cvon-hee          #+#    #+#              #
#    Updated: 2025/09/17 19:11:46 by asgalean         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### Project Info ###
NAME        = libft.a

### Compiler ###
CC          = clang
CFLAGS      = -Wall -Wextra -Werror -I includes/

### Colors ###
GREEN       = "\033[32m"
YELLOW      = "\033[33m"
RED         = "\033[31m"
RESET       = "\033[0m"

### Headers ###
HEADER      = $(addsuffix .h, $(addprefix includes/, libutils gnl))

### Source groups ###
FTIS        = alnum alpha ascii digit print space numeric
FTLST       = add_back add_front clear delone iter last map new size
FTMEM       = bzero calloc memchr memcmp memcpy memmove memset
FTPUT       = char_fd endl_fd nbr_fd str_fd
FTSTR       = chr cmp dup iteri join join_addspace lcat lcpy len mapi ncmp \
              nstr rchr trim join_3
FTSUB       = ft_str/ft_substr ft_str/ft_split
FTTO        = tolower toupper atoi long_atoi itoa
FTEX        = gnl/gnl

### Source files ###
SRC =  $(addsuffix .c, $(addprefix ft_is/ft_is, $(FTIS))) \
       $(addsuffix .c, $(addprefix ft_lst/ft_lst, $(FTLST))) \
       $(addsuffix .c, $(addprefix ft_mem/ft_, $(FTMEM))) \
       $(addsuffix .c, $(addprefix ft_put/ft_put, $(FTPUT))) \
       $(addsuffix .c, $(addprefix ft_str/ft_str, $(FTSTR)) $(FTSUB)) \
       $(addsuffix .c, $(addprefix ft_to/ft_, $(FTTO))) \
       $(addsuffix .c, $(addprefix ft_, $(FTEX)))

### Object files ###
OBJ = $(SRC:.c=.o)

### Rules ###
all: $(NAME)

$(NAME): $(OBJ)
	@echo $(YELLOW)"Compiling libft..."$(RESET)
	@ar rc $(NAME) $^
	@ranlib $(NAME)
	@echo $(GREEN)"LIBFT successfully compiled!"$(RESET)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo $(RED)"Cleaning libft object files..."$(RESET)
	@rm -f $(OBJ)

fclean: clean
	@echo $(RED)"Removing libft archive..."$(RESET)
	@rm -f $(NAME)
	@echo $(RED)"LIBFT successfully cleaned!"$(RESET)

re: fclean all

norm:
	norminette $(SRC) $(HEADER)

.PHONY: all clean fclean re norm