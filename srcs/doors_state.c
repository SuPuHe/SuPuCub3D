/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:44:42 by omizin            #+#    #+#             */
/*   Updated: 2025/11/07 16:45:14 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	door_state_opening(t_door *door)
{
	door->progress += DOOR_OPEN_SPEED;
	if (door->progress >= 1.0)
	{
		door->progress = 1.0;
		door->state = DOOR_OPEN;
		door->timer = DOOR_OPEN_TIME;
	}
}

void	door_state_open(t_door *door, t_game *game)
{
	door->timer--;
	if (door->timer <= 0)
	{
		if (!is_player_in_doorway(game, door))
			door->state = DOOR_CLOSING;
		else
			door->timer = 10;
	}
}

void	door_state_closing(t_door *door, t_game *game)
{
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
