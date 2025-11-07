/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:42:27 by omizin            #+#    #+#             */
/*   Updated: 2025/11/07 16:45:43 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
