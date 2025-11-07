/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:50:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 16:58:49 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
