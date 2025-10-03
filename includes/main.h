/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:05:21 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 17:37:53 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# define RESET		"\033[0m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"

# include "libft.h"
# include "get_next_line.h"
# include "structs.h"

# include <stdio.h>
# include <fcntl.h>

void	print_error(char *msg);
int		parsing_file(char *argv);
void	free_split(char **lines);
void	free_textures_path(t_textures textures);
t_game	*ft_game(void);

int		get_color(char *line, int helper);

char	**extract_map(char **split_file);

int		get_info(char **file, t_game *game);
int		get_player_pos(t_game *game);

// player_movement.c
void	handle_input(mlx_key_data_t keydata, void *params);
void	player_move(void *param);

#endif
