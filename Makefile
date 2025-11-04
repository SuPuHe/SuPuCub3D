# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/01 12:01:22 by omizin            #+#    #+#              #
#    Updated: 2025/10/01 12:01:22 by omizin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RESET_COLOR	= \033[0m			# Reset to default color
YELLOW		= \033[1;33m		# Brighter yellow
BLUE		= \033[1;34m		# Bright blue
GREEN		= \033[1;32m		# Bright green
RED			= \033[1;31m		# Bright red
CYAN		= \033[1;36m		# Bright cyan

NAME = cub

SUPULIB_DIR = SuPuLib
SUPULIB_REPO = https://github.com/SuPuHe/SuPuLib.git
SUPULIB_A = $(SUPULIB_DIR)/SuPuLib.a
INCS_DIR = includes
OBJS_DIR = objs
SRCS_DIR = srcs
MLX_DIR = mlx

MLX 	=	$(MLX_DIR)/build/libmlx42.a -ldl -lglfw -pthread -lm

INCLUDE = -I$(INCS_DIR) -I$(SUPULIB_DIR)/libft/includes -I$(SUPULIB_DIR)/ft_printf/includes -I$(SUPULIB_DIR)/get_next_line/includes
CFLAGS = -Wall -Wextra -Werror $(INCLUDE)
CC = cc

RM = rm -rf

SRCS = main.c utils.c parse.c parse_color.c get_map.c get_info.c player_movement.c minimap.c minimap_rays.c minimap_helpers.c raycast.c textures.c doors.c weapon.c

SRCS := $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: checkMLX $(SUPULIB_A) $(NAME)

$(SUPULIB_DIR):
	@git clone $(SUPULIB_REPO) $(SUPULIB_DIR)
	@echo "$(GREEN)SuPuLib cloned successfully$(RESET_COLOR)"

$(SUPULIB_A): | $(SUPULIB_DIR)
	@$(MAKE) -C $(SUPULIB_DIR)

$(NAME): $(SUPULIB_A) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(SUPULIB_A) $(MLX) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully$(RESET_COLOR)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

checkMLX:
	@if	[ ! -d "$(MLX_DIR)" ];	then	\
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR) && \
		cmake -B $(MLX_DIR)/build -S $(MLX_DIR) && \
		make -C $(MLX_DIR)/build; \
		if [ $$? -ne 0 ]; then \
			echo "$(RED)Error building MLX42$(RESET_COLOR)"; \
			exit 1; \
		fi; \
		echo "$(GREEN)MLX42 installed$(RESET_COLOR)"; \
	else \
		echo "$(GREEN)MLX42 already exists$(RESET_COLOR)"; \
	fi

clean:
	@$(RM) $(OBJS_DIR)
	@$(MAKE) -C $(SUPULIB_DIR) clean
	@echo "$(GREEN)Clean sucessfully$(RESET_COLOR)"

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(SUPULIB_DIR) fclean
	@echo "$(GREEN)Fclean sucessfully$(RESET_COLOR)"

re: fclean all

.PHONY: all clean fclean re
