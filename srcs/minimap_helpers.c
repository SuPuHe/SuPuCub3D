/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:51 by omizin            #+#    #+#             */
/*   Updated: 2025/11/07 12:22:34 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_minimap(t_game *game)
{
	int	width;
	int	height;

	game->minimap.minimap_size = 130 * (SCREEN_WIDTH / SCREEN_HEIGHT);
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
		if (ft_strchr(MAP_WALLS, c))
			m->color = 0xFF0000FF;
		else if (c == 'D')
			m->color = 0xdf8021FF;
		else if (ft_strchr(MAP_PLAYER_ZERO, c))
			m->color = 0x808080FF;
		else
			m->color = 0x000000FF;
	}
}
