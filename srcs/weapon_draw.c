/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 14:30:02 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	copy_weapon_pixels(mlx_image_t *img, mlx_texture_t *tex,
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

int	create_weapon_image(t_game *game, int weapon_x, int weapon_y,
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

void	update_weapon_frames(t_game *game, int src_x_right, int src_x_left)
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

int	create_weapon_images(t_game *game, int *weapon_x, int weapon_y)
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
