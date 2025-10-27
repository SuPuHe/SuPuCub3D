/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:26:08 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/27 12:39:39 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdint.h>
# include "../mlx/include/MLX42/MLX42.h"

#define DOOR_OPEN_SPEED 0.03
#define DOOR_CLOSE_SPEED 0.03
#define DOOR_OPEN_TIME 120      // frames to wait before closing
#define DOOR_INTERACTION_DIST 1.5

typedef enum e_door_state
{
	DOOR_CLOSED,
	DOOR_OPENING,
	DOOR_OPEN,
	DOOR_CLOSING
}	t_door_state;

typedef struct s_textures
{
	char			*north_path;
	char			*south_path;
	char			*west_path;
	char			*east_path;
	char			*door_path;
	mlx_texture_t	*north_tex;
	mlx_texture_t	*south_tex;
	mlx_texture_t	*west_tex;
	mlx_texture_t	*east_tex;
	mlx_texture_t	*door_tex;
	int				floor_color[3];
	int				ceil_color[3];
	uint32_t		floor;
	uint32_t		ceil;
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
	double			collision_radius;
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
	int				is_door;
}	t_raycast;

typedef struct s_image
{
	mlx_image_t		*north_img;
	mlx_image_t		*south_img;
	mlx_image_t		*west_img;
	mlx_image_t		*east_img;
	int				width;
	int				height;
}	t_image;

typedef struct s_mray
{
	int			num_rays;
	double		step_sample;
	double		max_dist;
	int			center;
	double		scale;
	int			i;
	int			radius_px;
	double		camera_x;
	int			screen_x;
	double		t_hit;
	double		t_circle;
	double		dir_len;
	double		t;
	double		wx;
	double		wy;
	int			px;
	int			py;
	int			dx;
	int			dy;
	double		hx;
	double		hy;
	int			hx_px;
	int			hy_py;
	int			ddx;
	int			ddy;
}	t_mray;

typedef struct s_minimap
{
	int				enabled;
	int				scale;
	int				center;
	int				radius;
	int				mx;
	int				my;
	int				map_x;
	int				map_y;
	double			start_x;
	double			start_y;
	double			world_x;
	double			world_y;
	uint32_t		color;
	mlx_image_t		*player_img;
	mlx_image_t		*img;
}	t_minimap;

// Update door struct in structs.h:
typedef struct s_door
{
	int				x;
	int				y;
	double			progress;        // 0.0 = closed, 1.0 = open
	t_door_state	state;
	int				timer;           // timer for auto-close
}	t_door;

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
	t_image			tx_images;
	t_door			doors[64];
	int				door_count;
}	t_game;

#endif
