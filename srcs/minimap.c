/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:25:40 by omizin            #+#    #+#             */
/*   Updated: 2025/10/14 13:42:50 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#define TILE_SIZE 16
#define MINIMAP_SCALE 0.5

void	init_minimap(t_game *game)
{
	int width = game->map.width * TILE_SIZE;
	int height = game->map.height * TILE_SIZE;

	game->minimap.img = mlx_new_image(game->mlx, width, height);
	game->minimap.player_img = mlx_new_image(game->mlx, width, height);

	if (!game->minimap.img || !game->minimap.player_img)
		print_error("Failed to create minimap images");

//	draw_minimap(game);
	mlx_image_to_window(game->mlx, game->minimap.img, 20, 20);
	mlx_image_to_window(game->mlx, game->minimap.player_img, 20, 20);
}

void	draw_square(mlx_image_t *img, int x, int y, uint32_t color)
{
	int i;
	int j;

	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			mlx_put_pixel(img, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (game->map.grid[y])
	{
		x = 0;
		while (game->map.grid[y][x])
		{
			if (game->map.grid[y][x] == '1')
				draw_square(game->minimap.img, x * TILE_SIZE * MINIMAP_SCALE, y * TILE_SIZE * MINIMAP_SCALE, 0xFF0000FF);
			else if (game->map.grid[y][x] == ' ')
				;
			else
				draw_square(game->minimap.img, x * TILE_SIZE * MINIMAP_SCALE, y * TILE_SIZE * MINIMAP_SCALE, 0x808080FF);
			x++;
		}
		y++;
	}
}

void	draw_player_direction(t_game *game)
{
	int px = game->player.x * TILE_SIZE * MINIMAP_SCALE;
	int py = game->player.y * TILE_SIZE * MINIMAP_SCALE;

	int	length = TILE_SIZE * MINIMAP_SCALE;
	int	end_x = px + (int)(game->player.dir_x * length);
	int	end_y = py + (int)(game->player.dir_y * length);

	int	steps = length * 2;
	int	i = 0;
	while (i < steps)
	{
		int	x = px + (end_x - px) * i / steps;
		int	y = py + (end_y - py) * i / steps;
		mlx_put_pixel(game->minimap.player_img, x, y, 0xFFFF00FF);
		i++;
	}
}

void	draw_player(t_game *game)
{
	int px = game->player.x * TILE_SIZE * MINIMAP_SCALE;
	int py = game->player.y * TILE_SIZE * MINIMAP_SCALE;
	int	size = 3;

	uint32_t	y = 0;
	while (y < game->minimap.player_img->height)
	{
		uint32_t	x = 0;
		while (x < game->minimap.player_img->width)
		{
			mlx_put_pixel(game->minimap.player_img, x, y, 0x00000000);
			x++;
		}
		y++;
	}

	int	y2 = -size;
	while (y2 <= size)
	{
		int	x2 = -size;
		while (x2 <= size)
		{
			mlx_put_pixel(game->minimap.player_img, px + x2, py + y2, 0x00FF00FF);
			x2++;
		}
		y2++;
	}
	draw_player_direction(game);
}

void	enable_minimap(t_game *game)
{
	game->minimap.enabled = 1;
	draw_minimap(game);
	draw_player(game);
}

void	disable_minimap(t_game *game)
{
	uint32_t	y;
	uint32_t	x;

	game->minimap.enabled = 0;
	y = 0;
	while (y < game->minimap.player_img->height)
	{
		x = 0;
		while (x < game->minimap.player_img->width)
		{
			mlx_put_pixel(game->minimap.player_img, x, y, 0x00000000);
			x++;
		}
		y++;
	}
	y = 0;
	while (y < game->minimap.img->height)
	{
		x = -1;
		while (++x < game->minimap.img->width)
			mlx_put_pixel(game->minimap.img, x, y, 0x00000000);
		y++;
	}
}

void	check_minimap(t_game *game)
{
	if (!game->minimap.enabled)
		enable_minimap(game);
	else
		disable_minimap(game);
}
