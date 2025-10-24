/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:25:40 by omizin            #+#    #+#             */
/*   Updated: 2025/10/24 11:45:42 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	draw_minimap(t_game *game)
{
	t_minimap	m;

	m.center = MINIMAP_SIZE / 2;
	m.radius = MINIMAP_SIZE / 2;
	m.start_x = game->player.x - (m.center / (TILE_SIZE * MINIMAP_SCALE));
	m.start_y = game->player.y - (m.center / (TILE_SIZE * MINIMAP_SCALE));
	m.my = 0;
	while (m.my < MINIMAP_SIZE)
	{
		m.mx = 0;
		while (m.mx < MINIMAP_SIZE)
		{
			m.world_x = m.start_x + (double)m.mx / (TILE_SIZE * MINIMAP_SCALE);
			m.world_y = m.start_y + (double)m.my / (TILE_SIZE * MINIMAP_SCALE);
			m.map_x = (int)m.world_x;
			m.map_y = (int)m.world_y;
			get_color_for_minimap(&m, game);
			mlx_put_pixel(game->minimap.img, m.mx, m.my, m.color);
			m.mx++;
		}
		m.my++;
	}
}

void	draw_circled_minimap(t_game *game)
{
	int	y;
	int	x;
	int	dx;
	int	dy;
	int	radius;

	radius = MINIMAP_SIZE / 2;
	y = 0;
	while (y < MINIMAP_SIZE)
	{
		x = 0;
		while (x < MINIMAP_SIZE)
		{
			dx = x - radius;
			dy = y - radius;
			if (dx * dx + dy * dy > radius * radius)
				mlx_put_pixel(game->minimap.img, x, y, 0x00000000);
			else if (dx * dx + dy * dy > (radius - 2) * (radius - 2))
				mlx_put_pixel(game->minimap.img, x, y, 0x00000088);
			x++;
		}
		y++;
	}
}

void	draw_player_on_minimap(t_game *game)
{
	int	center;
	int	dy;
	int	dx;

	center = MINIMAP_SIZE / 2;
	dy = -2;
	while (dy <= 2)
	{
		dx = -2;
		while (dx <= 2)
		{
			mlx_put_pixel(game->minimap.img,
				center + dx, center + dy, 0x00FF00FF);
			dx++;
		}
		dy++;
	}
}

// void	draw_player_direction(t_game *game)
// {
// 	int px = MINIMAP_SIZE / 2;
// 	int py = MINIMAP_SIZE / 2;

// 	int	length = TILE_SIZE * MINIMAP_SCALE;
// 	int	end_x = px + (int)(game->player.dir_x * length);
// 	int	end_y = py + (int)(game->player.dir_y * length);

// 	int	steps = length * 2;
// 	int	i = 0;
// 	while (i < steps)
// 	{
// 		int	x = px + (end_x - px) * i / steps;
// 		int	y = py + (end_y - py) * i / steps;
// 		mlx_put_pixel(game->minimap.img, x, y, 0xFFFF00FF);
// 		i++;
// 	}
// }


void	update_minimap(t_game *game)
{
	clean_minimap(game);
	draw_minimap(game);
	draw_circled_minimap(game);
	draw_rays_on_minimap(game);
	draw_player_on_minimap(game);
	//draw_player_direction(game);
}

void	check_minimap(t_game *game)
{
	if (!game->minimap.enabled)
	{
		game->minimap.enabled = 1;
		update_minimap(game);
	}
	else
	{
		game->minimap.enabled = 0;
		clean_minimap(game);
	}
}
