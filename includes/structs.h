/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:26:08 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/02 14:40:37 by omizin           ###   ########.fr       */
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
	uint32_t		floor_color;
	uint32_t		ceil_color;
}	t_textures;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
	int				fd;
}	t_map;

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
}	t_game;

#endif
