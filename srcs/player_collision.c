/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:50:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/10 12:53:58 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Checks if a given position on the map is
 * valid for the player to move into.
 *
 * This function ensures that the position (x, y) is within the map boundaries
 * and not blocked by walls or closed doors. It checks
 * for impassable tiles ('1' or '2'-'9'), and for door tiles ('D'), verifies
 * whether the door is sufficiently open.
 *
 * @param game Pointer to the main game structure containing the map and doors.
 * @param x The x-coordinate of the position to check.
 * @param y The y-coordinate of the position to check.
 * @return true if the position is valid for movement, false otherwise.
 */
bool	is_valid_position(t_game *game, double x, double y)
{
	int		map_x;
	int		map_y;
	t_door	*door;
	char	tile;

	if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
		return (false);
	map_x = (int)x;
	map_y = (int)y;
	tile = game->map.grid[map_y][map_x];
	if (tile == '1' || (tile >= '2' && tile <= '9'))
		return (false);
	if (tile == 'D')
	{
		door = find_door_at(game, map_x, map_y);
		if (door && door->progress < 0.9)
			return (false);
	}
	return (true);
}

/**
 * @brief Moves the player while handling collisions with walls and doors.
 *
 * This function updates the player's position to (new_x, new_y) only if the
 * movement does not cause the player to intersect with walls or closed doors.
 * A small collision radius around the player is used to prevent clipping into
 * obstacles.
 *
 * @param game Pointer to the main game structure
 * containing the player and map data.
 * @param new_x The desired new x-coordinate of the player.
 * @param new_y The desired new y-coordinate of the player.
 */
void	move_player_with_collision(t_game *game, double new_x, double new_y)
{
	double	radius;

	radius = game->player.collision_radius;
	if (new_x > game->player.x)
	{
		if (is_valid_position(game, new_x + radius, game->player.y))
			game->player.x = new_x;
	}
	else if (new_x < game->player.x)
	{
		if (is_valid_position(game, new_x - radius, game->player.y))
			game->player.x = new_x;
	}
	if (new_y > game->player.y)
	{
		if (is_valid_position(game, game->player.x, new_y + radius))
			game->player.y = new_y;
	}
	else if (new_y < game->player.y)
	{
		if (is_valid_position(game, game->player.x, new_y - radius))
			game->player.y = new_y;
	}
}
