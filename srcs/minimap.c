/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:25:40 by omizin            #+#    #+#             */
/*   Updated: 2025/10/14 15:58:45 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#define TILE_SIZE 16
#define MINIMAP_SCALE 0.5
#define MINIMAP_SIZE 200

void	init_minimap(t_game *game)
{
	int	width;
	int	height;

	width = game->map.width * TILE_SIZE;
	height = game->map.height * TILE_SIZE;
	game->minimap.img = mlx_new_image(game->mlx, MINIMAP_SIZE, MINIMAP_SIZE);
	game->minimap.player_img = mlx_new_image(game->mlx, width, height);
	if (!game->minimap.img || !game->minimap.player_img)
		print_error("Failed to create minimap images");
	mlx_image_to_window(game->mlx, game->minimap.img, 20, 20);
	mlx_image_to_window(game->mlx, game->minimap.player_img, 20, 20);
}

void	clean_minimap(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < MINIMAP_SIZE)
	{
		x = 0;
		while (x < MINIMAP_SIZE)
		{
			mlx_put_pixel(game->minimap.img, x, y, 0x00000000);
			x++;
		}
		y++;
	}
}

void	get_color_for_minimap(t_minimap *m, t_game *game)
{
	char	c;

	if (m->map_y < 0 || m->map_y >= game->map.height
		|| m->map_x < 0
		|| m->map_x >= (int)ft_strlen(game->map.grid[m->map_y]))
		m->color = 0x000000FF;
	else
	{
		c = game->map.grid[m->map_y][m->map_x];
		if (c == '1')
			m->color = 0xFF0000FF;
		else if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			m->color = 0x808080FF;
		else
			m->color = 0x000000FF;
	}
}

void	draw_minimap2(t_game *game)
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

void	draw_player_direction(t_game *game)
{
	int px = MINIMAP_SIZE / 2;
	int py = MINIMAP_SIZE / 2;

	int	length = TILE_SIZE * MINIMAP_SCALE;
	int	end_x = px + (int)(game->player.dir_x * length);
	int	end_y = py + (int)(game->player.dir_y * length);

	int	steps = length * 2;
	int	i = 0;
	while (i < steps)
	{
		int	x = px + (end_x - px) * i / steps;
		int	y = py + (end_y - py) * i / steps;
		mlx_put_pixel(game->minimap.img, x, y, 0xFFFF00FF);
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	clean_minimap(game);
	draw_minimap2(game);
	draw_circled_minimap(game);
	draw_player_on_minimap(game);
	draw_player_direction(game);
}

void	enable_minimap(t_game *game)
{
	game->minimap.enabled = 1;
	draw_minimap(game);
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
	clean_minimap(game);
}

void	check_minimap(t_game *game)
{
	if (!game->minimap.enabled)
		enable_minimap(game);
	else
		disable_minimap(game);
}
