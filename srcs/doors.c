/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:17:38 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/15 16:46:17 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	update_doors(void *param)
{
	int		i;
	t_game	*game;
	t_door	*door;

	(void)param;
	game = ft_game();
	i = 0;
	while (i < game->door_count)
	{
		door = &game->doors[i];
		if (door->is_openening && door->progress < 1.0)
			door->progress += 0.02; // Speed of opening the door
		else if (!door->is_openening && door->progress > 0.0)
			door->progress -= 0.02; // Speed of closing the door
		i++;
	}
}

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
				door->is_openening = false;
				game->door_count++;
			}
			x++;
		}
		y++;
	}
	// printf("Doors initialized: %d\n", game->door_count); // debug
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

void	is_door(void)
{
	int		map_x;
	int		map_y;
	t_game	*game;
	t_door	*door;

	game = ft_game();
	map_x = (int)(game->player.x + game->player.dir_x * 1.0);
	map_y = (int)(game->player.y + game->player.dir_y * 1.0);
	door = find_door_at(game, map_x, map_y);
	if (door)
	{
		door->is_openening = !door->is_openening;
		// printf("Door at (%d, %d) state: %s, progress: %f\n", door->x, door->y, door->is_openening ? "opening" : "closing", door->progress); // DEBUG
	}
}
