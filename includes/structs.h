/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:26:08 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/10 11:38:19 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdint.h>
# include "../mlx/include/MLX42/MLX42.h"

typedef enum e_door_state
{
	DOOR_CLOSED,
	DOOR_OPENING,
	DOOR_OPEN,
	DOOR_CLOSING
}	t_door_state;

typedef enum e_weapon_state
{
	WEAPON_IDLE,
	WEAPON_SHOOTING
}	t_weapon_state;

typedef enum e_weapon_side
{
	WEAPON_RIGHT,
	WEAPON_LEFT
}	t_weapon_side;

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
	mlx_texture_t	*gui_tex;
	mlx_texture_t	*door_tex;
	mlx_texture_t	**wall_textures;
	int				wall_tex_count;
	char			**wall_tex_paths;
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
	bool			moving;
	double			mouse_sensitivity;
	int				last_mouse_x;
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
	mlx_image_t		*gui_img;
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
	int				minimap_size;
	double			start_x;
	double			start_y;
	double			world_x;
	double			world_y;
	uint32_t		color;
	mlx_image_t		*player_img;
	mlx_image_t		*img;
}	t_minimap;

typedef struct s_door
{
	int				x;
	int				y;
	double			progress;
	t_door_state	state;
	int				timer;
}	t_door;

typedef struct s_weapon
{
	mlx_texture_t	*frames_right;
	mlx_texture_t	*frames_left;
	int				frame_count_right;
	int				frame_count_left;
	int				current_frame;
	int				last_drawn_frame_right;
	int				last_drawn_frame_left;
	int				frame_delay;
	int				frame_timer;
	int				frame_count;
	t_weapon_state	state;
	t_weapon_side	active_side;
	double			bob_offset;
	double			bob_timer;
	mlx_image_t		*img_right;
	mlx_image_t		*img_left;
	int				scaled_width;
	int				scaled_height;
	float			scale;
}	t_weapon;

typedef struct s_column_vars
{
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				y;
	int				x;
	int				tex_x;
	int				tex_y;
	double			step;
	double			tex_pos;
	mlx_texture_t	*texture;
	uint32_t		color;
	double			wall_x;
	char			wall_char;
	int				wall_type;
}	t_column_vars;

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
	int				mouse_enabled;
	t_weapon		weapon;
}	t_game;

#endif
