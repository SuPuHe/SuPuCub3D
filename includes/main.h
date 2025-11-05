/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:05:21 by omizin            #+#    #+#             */
/*   Updated: 2025/11/05 12:45:01 by omizin           ###   ########.fr       */
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

# define SCREEN_WIDTH 3440
# define SCREEN_HEIGHT 1440

# define TILE_SIZE 16
# define MINIMAP_SCALE 0.5
# define MINIMAP_SIZE 200

# define MAP_WALLS_DOORS "DF123456789"
# define MAP_WALLS "123456789"
# define MAP_PLAYER_ZERO "0NSEW"

# include "libft.h"
# include "get_next_line.h"
# include "structs.h"

# include <stdio.h>
# include <fcntl.h>
# include <math.h>

void	print_error(char *msg);
int		parsing_file(char *argv);
void	free_split(char **lines);
void	free_textures_path(t_textures textures);
t_game	*ft_game(void);
void	find_map_width(t_game *game);
char	*ft_strstr(const char *haystack, const char *needle);

int		get_color(char *line, int helper);

char	**extract_map(char **split_file);

int		get_info(char **file, t_game *game);
int		get_player_pos(t_game *game);
int		init_player_dir(t_game *game);

// player_movement.c
void	handle_input(mlx_key_data_t keydata, void *params);
void	player_move(void *param);
void	handle_mouse(void *param);
void	rotate_player(t_game *game, double angle);


//minimap
void	init_minimap(t_game *game);
void	draw_square(mlx_image_t *img, int x, int y, uint32_t color);
void	update_minimap(t_game *game);
void	draw_player(t_game *game);
void	check_minimap(t_game *game);

//minimap_rays.c
void	draw_rays_on_minimap(t_game *game);

//minimap_helpers.c
void	init_minimap(t_game *game);
void	clean_minimap(t_game *game);
void	get_color_for_minimap(t_minimap *m, t_game *game);

// raycast.c
void	render_3d_view(t_game *game);
void	init_ray(t_game *game, t_raycast *rc, int x);
void	calculate_step_and_side_dist(t_game *game, t_raycast *rc);
void	perform_dda(t_game *game, t_raycast *rc);
void	calculate_wall_distance(t_game *game, t_raycast *rc);

// textures.c
void	textures_load(void);
void	image_create(void);

//ui.c
void	init_ui();
void	animate_gui(t_game *game);

// doors.c
void	is_door(void);
void	update_doors(void *param);
t_door	*find_door_at(t_game *game, int x, int y);
void	init_doors(t_game *game);
void	interact_with_door(t_game *game);

// weapon.c
void	init_weapon(t_game *game);
void	update_weapon(void *param);
void	draw_weapon(void *param);
void	weapon_shoot(t_game *game);
#endif
