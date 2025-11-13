/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_player_info.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:36:37 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/11 11:37:44 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Finds the player's starting position in the map.
 *
 * Iterates through the map grid to locate a cell containing
 * a player start direction ('N', 'S', 'E', 'W'). Sets the
 * player's x and y coordinates to the center of that cell.
 *
 * @param game Pointer to the game structure.
 * @return int Returns 1 if exactly one player position is found, 0 otherwise.
 */
int	get_player_pos(t_game *game)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	found = 0;
	while (ft_game()->map.grid[i])
	{
		j = 0;
		while (ft_game()->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S'
				|| game->map.grid[i][j] == 'E' || game->map.grid[i][j] == 'W')
			{
				ft_game()->player.x = (double)j + 0.5;
				ft_game()->player.y = (double)i + 0.5;
				found++;
			}
			j++;
		}
		i++;
	}
	if (found != 1)
		return (0);
	return (1);
}

/**
 * @brief Initializes the player's direction vector and camera plane.
 *
 * Sets the player's dir_x/dir_y and plane_x/plane_y values based
 * on the starting map cell ('N', 'S', 'E', 'W') to determine the
 * initial viewing direction and field of view.
 *
 * @param game Pointer to the game structure.
 * @return int Returns 1 on success.
 */
int	init_player_dir(t_game *game)
{
	char	dir;

	dir = game->map.grid[(int)game->player.y][(int)game->player.x];
	if (dir == 'E')
	{
		game->player.dir_x = 1.0;
		game->player.plane_y = 0.75;
	}
	else if (dir == 'W')
	{
		game->player.dir_x = -1.0;
		game->player.plane_y = -0.75;
	}
	else if (dir == 'N')
	{
		game->player.dir_y = -1.0;
		game->player.plane_x = 0.75;
	}
	else if (dir == 'S')
	{
		game->player.dir_y = 1.0;
		game->player.plane_x = -0.75;
	}
	return (1);
}
