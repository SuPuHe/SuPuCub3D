/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:17:38 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/10 13:13:08 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Checks if the player is currently inside a door tile.
 *
 * Compares the player's integer map coordinates with the door's
 * coordinates.
 *
 * @param game Pointer to the game structure.
 * @param door Pointer to the door structure to check.
 * @return true if the player is in the doorway, false otherwise.
 */
bool	is_player_in_doorway(t_game *game, t_door *door)
{
	int		player_map_x;
	int		player_map_y;

	player_map_x = (int)game->player.x;
	player_map_y = (int)game->player.y;
	if (player_map_x == door->x && player_map_y == door->y)
		return (true);
	return (false);
}

/**
 * @brief Updates all doors in the game based on their current states.
 *
 * Iterates over all doors and advances their state:
 * - DOOR_OPENING → door_state_opening()
 * - DOOR_OPEN → door_state_open()
 * - DOOR_CLOSING → door_state_closing()
 *
 * @param param Pointer to the game structure
 * (void* for loop hook compatibility).
 */
void	update_doors(void *param)
{
	int		i;
	t_game	*game;
	t_door	*door;

	game = (t_game *)param;
	i = 0;
	while (i < game->door_count)
	{
		door = &game->doors[i];
		if (door->state == DOOR_OPENING)
			door_state_opening(door);
		else if (door->state == DOOR_OPEN)
			door_state_open(door, game);
		else if (door->state == DOOR_CLOSING)
			door_state_closing(door, game);
		i++;
	}
}

/**
 * @brief Calculates the distance between the player and a door.
 *
 * Uses Euclidean distance from the center of the door to the player.
 *
 * @param game Pointer to the game structure.
 * @param door Pointer to the door structure.
 * @return Distance between player and door.
 */
static double	get_distance(t_game *game, t_door *door)
{
	double	dx;
	double	dy;
	double	dist;

	dx = (door->x + 0.5) - game->player.x;
	dy = (door->y + 0.5) - game->player.y;
	dist = sqrt(dx * dx + dy * dy);
	return (dist);
}

/**
 * @brief Toggles a door's state based on its current state.
 *
 * Logic:
 * - CLOSED → OPENING
 * - OPEN → CLOSING (if player not in doorway)
 * - OPENING → CLOSING
 * - CLOSING → OPENING
 *
 * @param game Pointer to the game structure.
 * @param door Pointer to the door structure to change state.
 */
static void	change_door_state(t_game *game, t_door *door)
{
	if (door->state == DOOR_CLOSED)
		door->state = DOOR_OPENING;
	else if (door->state == DOOR_OPEN)
	{
		if (!is_player_in_doorway(game, door))
			door->state = DOOR_CLOSING;
	}
	else if (door->state == DOOR_OPENING)
	{
		door->state = DOOR_CLOSING;
	}
	else if (door->state == DOOR_CLOSING)
	{
		door->state = DOOR_OPENING;
	}
}

/**
 * @brief Handles player interaction with a door in front of them.
 *
 * Casts a short distance in the player's facing direction to find a
 * door. If the player is close enough, toggles the door's state.
 *
 * @param game Pointer to the game structure.
 */
void	interact_with_door(t_game *game)
{
	int		map_x;
	int		map_y;
	t_door	*door;
	double	dist;

	map_x = (int)(game->player.x + game->player.dir_x * DOOR_INTERACTION_DIST);
	map_y = (int)(game->player.y + game->player.dir_y * DOOR_INTERACTION_DIST);
	door = find_door_at(game, map_x, map_y);
	if (!door)
		return ;
	dist = get_distance(game, door);
	if (dist > DOOR_INTERACTION_DIST)
		return ;
	change_door_state(game, door);
}
