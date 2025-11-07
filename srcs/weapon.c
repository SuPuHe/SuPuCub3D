/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 14:20:25 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_weapon(t_game *game)
{
	game->weapon.frame_count_right = 5;
	game->weapon.frames_right = mlx_load_png("textures/sprites/right_gun.png");
	game->weapon.frame_count_left = 5;
	game->weapon.frames_left = mlx_load_png("textures/sprites/left_gun.png");
	if (!game->weapon.frames_right)
		return ;
	if (!game->weapon.frames_left)
		return ;
	game->weapon.current_frame = 0;
	game->weapon.last_drawn_frame_right = -1;
	game->weapon.last_drawn_frame_left = -1;
	game->weapon.frame_delay = 5;
	game->weapon.frame_timer = 0;
	game->weapon.state = WEAPON_IDLE;
	game->weapon.active_side = WEAPON_RIGHT;
	game->weapon.bob_offset = 0.0;
	game->weapon.bob_timer = 0.0;
	game->weapon.img_right = NULL;
	game->weapon.img_left = NULL;
}
void	update_weapon_bobbing(t_game *game)
{
	bool	is_moving;
	double	bob_speed;
	double	bob_amount;
	float	screen_scale;

	if (game->player.move.forward || game->player.move.backward)
		is_moving = true;
	else
		is_moving = false;
	if (is_moving)
	{
		bob_speed = 0.15;
		screen_scale = (float)SCREEN_HEIGHT / 1080.0f;
		bob_amount = 10.0 * screen_scale;
		game->weapon.bob_timer += bob_speed;
		game->weapon.bob_offset = sin(game->weapon.bob_timer) * bob_amount;
	}
	else
	{
		game->weapon.bob_offset *= 0.9;
		if (fabs(game->weapon.bob_offset) < 0.1)
			game->weapon.bob_offset = 0.0;
	}
}

void	update_weapon_animation(t_game *game)
{
	game->weapon.frame_timer++;
	if (game->weapon.state == WEAPON_SHOOTING)
	{
		if (game->weapon.active_side == WEAPON_RIGHT)
			game->weapon.frame_count = game->weapon.frame_count_right;
		else
			game->weapon.frame_count = game->weapon.frame_count_left;
		if (game->weapon.frame_timer >= game->weapon.frame_delay)
		{
			game->weapon.frame_timer = 0;
			game->weapon.current_frame++;
			if (game->weapon.current_frame >= game->weapon.frame_count)
			{
				game->weapon.current_frame = 0;
				game->weapon.state = WEAPON_IDLE;
				if (game->weapon.active_side == WEAPON_RIGHT)
					game->weapon.active_side = WEAPON_LEFT;
				else
					game->weapon.active_side = WEAPON_RIGHT;
			}
		}
	}
	else
		game->weapon.current_frame = 0;
}

void	update_weapon(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	update_weapon_bobbing(game);
	update_weapon_animation(game);
}

static void	calculate_weapon_scale(t_game *game)
{
	int		frame_width;
	int		frame_height;
	float	screen_scale;

	frame_width = game->weapon.frames_right->width
		/ game->weapon.frame_count_right;
	frame_height = game->weapon.frames_right->height;
	screen_scale = (float)SCREEN_WIDTH / 1920.0f;
	game->weapon.scale = 0.5f * screen_scale;
	game->weapon.scaled_width = (int)(frame_width * game->weapon.scale);
	game->weapon.scaled_height = (int)(frame_height * game->weapon.scale);
}

static void	get_frame_offsets(t_game *game, int *src_x_right, int *src_x_left)
{
	int	frame_width;
	int	left_offset;

	frame_width = game->weapon.frames_right->width
		/ game->weapon.frame_count_right;
	left_offset = 90;
	if (game->weapon.state == WEAPON_SHOOTING)
	{
		if (game->weapon.active_side == WEAPON_RIGHT)
		{
			*src_x_right = game->weapon.current_frame * frame_width;
			*src_x_left = left_offset;
		}
		else
		{
			*src_x_right = 0;
			*src_x_left = game->weapon.current_frame * frame_width
				+ left_offset;
		}
	}
	else
	{
		*src_x_right = 0;
		*src_x_left = left_offset;
	}
}

static void	copy_weapon_pixels(mlx_image_t *img, mlx_texture_t *tex,
				int src_x, t_game *game)
{
	int	x;
	int	y;
	int	src_idx;
	int	dst_idx;

	y = 0;
	while (y < game->weapon.scaled_height)
	{
		x = 0;
		while (x < game->weapon.scaled_width)
		{
			src_idx = (((int)(y / game->weapon.scale) * tex->width)
					+ (src_x + (int)(x / game->weapon.scale))) * 4;
			dst_idx = (y * game->weapon.scaled_width + x) * 4;
			img->pixels[dst_idx + 0] = tex->pixels[src_idx + 0];
			img->pixels[dst_idx + 1] = tex->pixels[src_idx + 1];
			img->pixels[dst_idx + 2] = tex->pixels[src_idx + 2];
			img->pixels[dst_idx + 3] = tex->pixels[src_idx + 3];
			x++;
		}
		y++;
	}
}

static int	create_weapon_image(t_game *game, int weapon_x, int weapon_y,
				mlx_image_t **img)
{
	*img = mlx_new_image(game->mlx, game->weapon.scaled_width,
			game->weapon.scaled_height);
	if (!*img)
		return (0);
	if (!mlx_image_to_window(game->mlx, *img, weapon_x, weapon_y))
		return (0);
	mlx_set_instance_depth(&(*img)->instances[0], 10000);
	(*img)->enabled = true;
	return (1);
}

static void	update_weapon_frames(t_game *game, int src_x_right,
				int src_x_left)
{
	if (game->weapon.last_drawn_frame_right != src_x_right)
	{
		copy_weapon_pixels(game->weapon.img_right,
			game->weapon.frames_right, src_x_right, game);
		game->weapon.last_drawn_frame_right = src_x_right;
	}
	if (game->weapon.last_drawn_frame_left != src_x_left)
	{
		copy_weapon_pixels(game->weapon.img_left,
			game->weapon.frames_left, src_x_left, game);
		game->weapon.last_drawn_frame_left = src_x_left;
	}
}

static void	setup_weapon_positions(t_game *game, int *weapon_x, int *weapon_y)
{
	float	screen_scale;

	screen_scale = (float)SCREEN_WIDTH / 1920.0f;
	weapon_x[0] = SCREEN_WIDTH / 2 + (int)(100 * screen_scale);
	weapon_x[1] = SCREEN_WIDTH / 2 - game->weapon.scaled_width
		- (int)(100 * screen_scale);
	*weapon_y = SCREEN_HEIGHT - (int)(game->weapon.scaled_height * 0.75f)
		+ (int)game->weapon.bob_offset;
}

static int	create_weapon_images(t_game *game, int *weapon_x, int weapon_y)
{
	if (!game->weapon.img_right)
	{
		if (!create_weapon_image(game, weapon_x[0], weapon_y,
				&game->weapon.img_right))
			return (0);
	}
	if (!game->weapon.img_left)
	{
		if (!create_weapon_image(game, weapon_x[1], weapon_y,
				&game->weapon.img_left))
			return (0);
	}
	return (1);
}

void	draw_weapon(void *param)
{
	t_game	*game;
	int		src_x_right;
	int		src_x_left;
	int		weapon_x[2];
	int		weapon_y;

	game = (t_game *)param;
	if (!game->weapon.frames_right || !game->weapon.frames_left)
		return ;
	calculate_weapon_scale(game);
	get_frame_offsets(game, &src_x_right, &src_x_left);
	setup_weapon_positions(game, weapon_x, &weapon_y);
	if (!create_weapon_images(game, weapon_x, weapon_y))
		return ;
	update_weapon_frames(game, src_x_right, src_x_left);
	game->weapon.img_right->instances[0].x = weapon_x[0];
	game->weapon.img_right->instances[0].y = weapon_y;
	game->weapon.img_left->instances[0].x = weapon_x[1];
	game->weapon.img_left->instances[0].y = weapon_y;
	game->weapon.img_right->enabled = true;
	game->weapon.img_left->enabled = true;
}

void	weapon_shoot(t_game *game)
{
	if (game->weapon.state == WEAPON_IDLE)
	{
		game->weapon.state = WEAPON_SHOOTING;
		game->weapon.current_frame = 0;
		game->weapon.frame_timer = 0;
	}
}
