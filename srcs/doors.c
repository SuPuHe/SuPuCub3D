/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:17:38 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/17 11:46:25 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_doors(t_game *game)
{
	int		y;
	int		x;
	t_door	*door;

	game->door_count = 0;
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (game->map.grid[y][x] == 'D')
			{
				if (game->door_count >= 64)
					break ;
				door = &game->doors[game->door_count];
				door->x = x;
				door->y = y;
				door->progress = 0.0;
				door->state = DOOR_CLOSED;
				door->timer = 0;
				game->door_count++;
			}
			x++;
		}
		y++;
	}
}

// Find door at specific coordinates
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

// Check if player is blocking door from closing
static bool	is_player_in_doorway(t_game *game, t_door *door)
{
	int		player_map_x;
	int		player_map_y;

	player_map_x = (int)game->player.x;
	player_map_y = (int)game->player.y;

	// Check if player is in the same cell as door
	if (player_map_x == door->x && player_map_y == door->y)
		return (true);

	return (false);
}

// Update all doors (call every frame)
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
		{
			door->progress += DOOR_OPEN_SPEED;
			if (door->progress >= 1.0)
			{
				door->progress = 1.0;
				door->state = DOOR_OPEN;
				door->timer = DOOR_OPEN_TIME;
			}
		}
		else if (door->state == DOOR_OPEN)
		{
			door->timer--;
			if (door->timer <= 0)
			{
				// Start closing if player not in doorway
				if (!is_player_in_doorway(game, door))
					door->state = DOOR_CLOSING;
				else
					door->timer = 10; // retry in a bit
			}
		}
		else if (door->state == DOOR_CLOSING)
		{
			// Check if player entered doorway while closing
			if (is_player_in_doorway(game, door))
			{
				door->state = DOOR_OPENING;
			}
			else
			{
				door->progress -= DOOR_CLOSE_SPEED;
				if (door->progress <= 0.0)
				{
					door->progress = 0.0;
					door->state = DOOR_CLOSED;
				}
			}
		}
		i++;
	}
}

// Interact with door (E key or use button)
void	interact_with_door(t_game *game)
{
	int		map_x;
	int		map_y;
	t_door	*door;
	double	dx;
	double	dy;
	double	dist;

	// Check tile in front of player
	map_x = (int)(game->player.x + game->player.dir_x * DOOR_INTERACTION_DIST);
	map_y = (int)(game->player.y + game->player.dir_y * DOOR_INTERACTION_DIST);

	door = find_door_at(game, map_x, map_y);
	if (!door)
		return ;

	// Calculate distance to door center
	dx = (door->x + 0.5) - game->player.x;
	dy = (door->y + 0.5) - game->player.y;
	dist = sqrt(dx * dx + dy * dy);

	// Only interact if close enough
	if (dist > DOOR_INTERACTION_DIST)
		return ;

	// Toggle door state
	if (door->state == DOOR_CLOSED)
	{
		door->state = DOOR_OPENING;
	}
	else if (door->state == DOOR_OPEN)
	{
		// Manual close
		if (!is_player_in_doorway(game, door))
			door->state = DOOR_CLOSING;
	}
	else if (door->state == DOOR_OPENING)
	{
		// Stop opening and start closing
		door->state = DOOR_CLOSING;
	}
	else if (door->state == DOOR_CLOSING)
	{
		// Reverse to opening
		door->state = DOOR_OPENING;
	}
}
