/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:17:44 by omizin            #+#    #+#             */
/*   Updated: 2025/10/31 13:43:58 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#define FRAME_SIZE 235
#define FISTS_WIDTH 390
#define FISTS_HEIGHT 213
#define FISTS_SCALE 2.5

void	init_ui(void)
{
	t_game	*game;

	game = ft_game();
	game->tx_images.gui_img = mlx_new_image(game->mlx, FRAME_SIZE, FRAME_SIZE);
	game->tx_images.fists_img = mlx_new_image(game->mlx,
		FISTS_WIDTH * FISTS_SCALE, FISTS_HEIGHT * FISTS_SCALE);
	if (!game->tx_images.gui_img)
		print_error("Failed to create GUI frame image");
	mlx_image_to_window(game->mlx, game->tx_images.gui_img,
		SCREEN_WIDTH / 2 - 125, 250);
	mlx_image_to_window(game->mlx, game->tx_images.fists_img,
		SCREEN_WIDTH / 2 - (FISTS_WIDTH * FISTS_SCALE) / 2,
		SCREEN_HEIGHT - (FISTS_HEIGHT * FISTS_SCALE) - 100);
}

void	gui_set_frame(t_game *game, int frame_index)
{
	int			sheet_x;
	int			x;
	int			y;
	int			src;
	uint32_t	color;

	sheet_x = frame_index * FRAME_SIZE;
	y = 0;
	while (y < FRAME_SIZE)
	{
		x = 0;
		while (x < FRAME_SIZE)
		{
			src = ((y * 9165) + (sheet_x + x)) * 4;
			color = (game->textures.gui_tex->pixels[src + 0] << 24)
				| (game->textures.gui_tex->pixels[src + 1] << 16)
				| (game->textures.gui_tex->pixels[src + 2] << 8)
				| (game->textures.gui_tex->pixels[src + 3]);
			mlx_put_pixel(game->tx_images.gui_img, x, y, color);
			x++;
		}
		y++;
	}
}

void	fists_set_frame(t_game *game, int frame_index)
{
	int			sheet_x, src_x, src_y, x, y;
	int			dst_x, dst_y;
	int			src;
	uint32_t	color;
	float		scale = FISTS_SCALE;

	y = 0;
	while (y < (int)game->tx_images.fists_img->height)
	{
		x = 0;
		while (x < (int)game->tx_images.fists_img->width)
		{
			mlx_put_pixel(game->tx_images.fists_img, x, y, 0x00000000);
			x++;
		}
		y++;
	}
	sheet_x = frame_index * FISTS_WIDTH;
	src_y = 0;
	while (src_y < FISTS_HEIGHT)
	{
		src_x = 0;
		while (src_x < FISTS_WIDTH)
		{
			if (sheet_x + src_x >= (int)game->textures.fists_tex->width)
				break;
			src = ((src_y * game->textures.fists_tex->width) + (sheet_x + src_x)) * 4;
			color = (game->textures.fists_tex->pixels[src + 0] << 24)
				| (game->textures.fists_tex->pixels[src + 1] << 16)
				| (game->textures.fists_tex->pixels[src + 2] << 8)
				| (game->textures.fists_tex->pixels[src + 3]);
			dst_y = src_y * scale;
			while (dst_y < (src_y + 1) * scale)
			{
				dst_x = src_x * scale;
				while (dst_x < (src_x + 1) * scale)
				{
					if ((uint32_t)dst_x < game->tx_images.fists_img->width
						&& (uint32_t)dst_y < game->tx_images.fists_img->height)
						mlx_put_pixel(game->tx_images.fists_img, dst_x, dst_y, color);
					dst_x++;
				}
				dst_y++;
			}
			src_x++;
		}
		src_y++;
	}
}

void	animate_fists(t_game *game)
{
	static double	last_time = 0;
	static int		frame = 0;
	double			now;

	now = mlx_get_time();
	if (now - last_time > 0.3)
	{
		fists_set_frame(game, frame);
		frame = (frame + 1) % 3;
		last_time = now;
	}
}

void	animate_gui(t_game *game)
{
	static double	last_time = 0;
	static int		frame = 0;
	double			now;

	now = mlx_get_time();
	if (now - last_time > 0.1)
	{
		gui_set_frame(game, frame);
		frame = (frame + 1) % 26;
		last_time = now;
	}
}
