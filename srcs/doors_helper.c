/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:42:27 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:12:22 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes a single door's position and state.
 *
 * Sets the door's coordinates, initial progress, state (closed),
 * and timer. Increments the game's door count.
 *
 * @param game Pointer to the game structure.
 * @param x X-coordinate of the door on the map.
 * @param y Y-coordinate of the door on the map.
 */
static void	init_door_positions(t_game *game, int x, int y)
{
	t_door	*door;

	door = &game->doors[game->door_count];
	door->x = x;
	door->y = y;
	door->progress = 0.0;
	door->state = DOOR_CLOSED;
	door->timer = 0;
	game->door_count++;
}

/**
 * @brief Scans the map and initializes all doors found.
 *
 * Iterates through the map grid and calls init_door_positions() for
 * each door ('D') tile. Limits total doors to 64.
 *
 * @param game Pointer to the game structure.
 */
void	init_doors(t_game *game)
{
	int		y;
	int		x;

	game->door_count = 0;
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.grid[y][x])
		{
			if (game->map.grid[y][x] == 'D')
			{
				if (game->door_count >= 64)
					break ;
				init_door_positions(game, x, y);
			}
			x++;
		}
		y++;
	}
}

/**
 * @brief Finds a door at given map coordinates.
 *
 * Searches through the game's door array to find a door at the
 * specified (x, y) location.
 *
 * @param game Pointer to the game structure.
 * @param x X-coordinate to search for.
 * @param y Y-coordinate to search for.
 * @return Pointer to the door if found, NULL otherwise.
 */
t_door	*find_door_at(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->door_count)
	{
		if (game->doors[i].x == x && game->doors[i].y == y)
			return (&game->doors[i]);
		i++;
	}
	return (NULL);
}
