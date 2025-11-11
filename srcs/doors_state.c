/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:44:42 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:12:54 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Updates a door in the opening state.
 *
 * Increases the door's progress according to DOOR_OPEN_SPEED.
 * When fully open (progress >= 1.0), sets the state to DOOR_OPEN
 * and initializes the door's open timer.
 *
 * @param door Pointer to the door structure.
 */
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

/**
 * @brief Updates a door in the open state.
 *
 * Decrements the door's timer. If the timer reaches 0 and the
 * player is not in the doorway, the door starts closing.
 * Otherwise, the timer is refreshed to keep the door open.
 *
 * @param door Pointer to the door structure.
 * @param game Pointer to the game structure.
 */
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

/**
 * @brief Updates a door in the closing state.
 *
 * If the player is in the doorway, the door reopens.
 * Otherwise, the door's progress decreases according to
 * DOOR_CLOSE_SPEED. When fully closed (progress <= 0), the
 * state is set to DOOR_CLOSED.
 *
 * @param door Pointer to the door structure.
 * @param game Pointer to the game structure.
 */
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
