/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:25:40 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:04:08 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Draws the minimap by sampling the world around the player.
 *
 * Iterates over each pixel of the minimap image,
 * converts it to world coordinates, determines the color
 * based on the map tile, and draws the pixel.
 *
 * @param game Pointer to the main game state.
 */
void	draw_minimap(t_game *game)
{
	t_minimap	m;

	m.center = game->minimap.minimap_size / 2;
	m.radius = game->minimap.minimap_size / 2;
	m.start_x = game->player.x - (m.center / (TILE_SIZE * MINIMAP_SCALE));
	m.start_y = game->player.y - (m.center / (TILE_SIZE * MINIMAP_SCALE));
	m.my = 0;
	while (m.my < game->minimap.minimap_size)
	{
		m.mx = 0;
		while (m.mx < game->minimap.minimap_size)
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

/**
 * @brief Draws a circular border and background for the minimap.
 *
 * Ensures the minimap is displayed as a circle, with an optional border effect.
 *
 * @param game Pointer to the main game state.
 */
void	draw_circled_minimap(t_game *game)
{
	int	y;
	int	x;
	int	dx;
	int	dy;
	int	radius;

	radius = game->minimap.minimap_size / 2;
	y = 0;
	while (y < game->minimap.minimap_size)
	{
		x = 0;
		while (x < game->minimap.minimap_size)
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

/**
 * @brief Draws the player’s position on the minimap.
 *
 * Draws a small square (5x5 pixels) in the center of the minimap representing
 * the player’s current location.
 *
 * @param game Pointer to the main game state.
 */
void	draw_player_on_minimap(t_game *game)
{
	int	center;
	int	dy;
	int	dx;

	center = game->minimap.minimap_size / 2;
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

/**
 * @brief Updates the minimap display.
 *
 * Clears the previous minimap, redraws the map, circular mask, rays, and
 * the player indicator.
 *
 * @param game Pointer to the main game state.
 */
void	update_minimap(t_game *game)
{
	clean_minimap(game);
	draw_minimap(game);
	draw_circled_minimap(game);
	draw_rays_on_minimap(game);
	draw_player_on_minimap(game);
}

/**
 * @brief Toggles the minimap on or off.
 *
 * If the minimap is currently disabled, enables it and updates the display.
 * If it is enabled, disables it and clears the minimap image.
 *
 * @param game Pointer to the main game state.
 */
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
