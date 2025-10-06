/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:26:08 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/06 12:45:51 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdint.h>
# include "../mlx/include/MLX42/MLX42.h"

typedef struct s_textures
{
	char			*north_path;
	char			*south_path;
	char			*west_path;
	char			*east_path;
	mlx_texture_t	*north_tex;
	mlx_texture_t	*south_tex;
	mlx_texture_t	*west_tex;
	mlx_texture_t	*east_tex;
	int				floor_color[3];
	int				ceil_color[3];
}	t_textures;

typedef struct s_map
{
	char			**grid;
	char			**copy_map;
	int				width;
	int				height;
	int				fd;
}	t_map;

typedef struct s_move
{
	bool			forward;
	bool			backward;
	bool			left;
	bool			right;
	bool			turn_left;
	bool			turn_right;
}	t_move;

typedef struct s_player
{
	double			x;
	double			y;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	double			move_speed;
	double			rot_speed;
	t_move			move;
}	t_player;

typedef struct s_raycast
{
	double			ray_dir_x;
	double			ray_dir_y;
	int				map_x;
	int				map_y;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	double			perp_wall_dist;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
}	t_raycast;

typedef struct s_image
{
	mlx_image_t		*img;
	int				width;
	int				height;
}	t_image;

typedef struct s_minimap
{
	int				enabled;
	int				scale;
	mlx_image_t		*img;
	mlx_image_t		*player_img;
}	t_minimap;

typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*win_img;
	t_map			map;
	t_player		player;
	t_textures		textures;
	t_raycast		rc;
	t_minimap		minimap;
	int				exit;
}	t_game;

#endif
