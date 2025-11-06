/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:17:44 by omizin            #+#    #+#             */
/*   Updated: 2025/11/06 12:24:45 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#define FRAME_SIZE 235

void	init_ui(void)
{
	t_game	*game;

	game = ft_game();
	game->tx_images.gui_img = mlx_new_image(game->mlx, FRAME_SIZE, FRAME_SIZE);
	if (!game->tx_images.gui_img)
		print_error("Failed to create GUI frame image");
	mlx_image_to_window(game->mlx, game->tx_images.gui_img,
		SCREEN_WIDTH / 2 - 125, 250);
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

void	animate_gui(void *params)
{
	static double	last_time = 0;
	static int		frame = 0;
	double			now;
	t_game			*game;

	game = (t_game *)params;
	now = mlx_get_time();
	if (now - last_time > 0.1)
	{
		gui_set_frame(game, frame);
		frame = (frame + 1) % 26;
		last_time = now;
	}
}
