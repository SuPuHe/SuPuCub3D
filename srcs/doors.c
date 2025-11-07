/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:17:38 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 16:47:49 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
