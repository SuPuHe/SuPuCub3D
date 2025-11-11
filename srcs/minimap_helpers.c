/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:51 by omizin            #+#    #+#             */
/*   Updated: 2025/11/11 10:33:30 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes the minimap and its player overlay images.
 *
 * Creates two MLX images:
 * 1. minimap.img – the circular minimap itself
 * 2. minimap.player_img – a full map overlay for
 * drawing player position and rays
 *
 * Places both images in the window at a fixed position.
 *
 * @param game Pointer to the main game state.
 */
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
	{
		print_error("Failed to create minimap images");
		exit_game(1);
	}
	mlx_image_to_window(game->mlx, game->minimap.img, 20, 20);
	mlx_image_to_window(game->mlx, game->minimap.player_img, 20, 20);
}

/**
 * @brief Clears the minimap image by filling it with transparent pixels.
 *
 * Iterates over every pixel in the minimap and sets it to 0x00000000
 * to make it fully transparent.
 *
 * @param game Pointer to the main game state.
 */
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

/**
 * @brief Determines the color of a pixel on the minimap based on the map tile.
 *
 * Maps walls, doors, empty tiles, and player starting positions
 * to specific RGBA colors. If coordinates are outside the map boundaries,0
 *  the color is set to blue (0x000000FF).
 *
 * @param m Pointer to the minimap state structure for the current pixel.
 * @param game Pointer to the main game state.
 */
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
