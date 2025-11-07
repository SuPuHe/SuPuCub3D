/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_doors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 16:32:39 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	get_door_orientation(t_game *game, t_raycast *rc)
{
	if ((rc->map_x > 0 && game->map.grid[rc->map_y][rc->map_x - 1] == '1')
		|| (rc->map_x < game->map.width - 1
			&& game->map.grid[rc->map_y][rc->map_x + 1] == '1'))
		return (1);
	if ((rc->map_y > 0 && game->map.grid[rc->map_y - 1][rc->map_x] == '1')
		|| (rc->map_y < game->map.height - 1
			&& game->map.grid[rc->map_y + 1][rc->map_x] == '1'))
		return (0);
	return (0);
}

void	check_vertical_door(t_game *game, t_raycast *rc, t_door *door)
{
	double	dist_to_center;
	double	ray_pos;
	double	door_pos;

	if (rc->ray_dir_x == 0)
		return ;
	dist_to_center = ((double)rc->map_x + 0.5 - game->player.x)
		/ rc->ray_dir_x;
	if (dist_to_center <= 0)
		return ;
	ray_pos = game->player.y + dist_to_center * rc->ray_dir_y;
	door_pos = ray_pos - (double)rc->map_y;
	if (door_pos >= 0.0 && door_pos <= 1.0
		&& door_pos <= (1.0 - door->progress))
	{
		rc->is_door = 1;
		rc->hit = 1;
		rc->perp_wall_dist = fabs(dist_to_center);
		rc->side = 0;
	}
}

void	check_horizontal_door(t_game *game, t_raycast *rc, t_door *door)
{
	double	dist_to_center;
	double	ray_pos;
	double	door_pos;

	if (rc->ray_dir_y == 0)
		return ;
	dist_to_center = ((double)rc->map_y + 0.5 - game->player.y)
		/ rc->ray_dir_y;
	if (dist_to_center <= 0)
		return ;
	ray_pos = game->player.x + dist_to_center * rc->ray_dir_x;
	door_pos = ray_pos - (double)rc->map_x;
	if (door_pos >= 0.0 && door_pos <= 1.0
		&& door_pos <= (1.0 - door->progress))
	{
		rc->is_door = 1;
		rc->hit = 1;
		rc->perp_wall_dist = fabs(dist_to_center);
		rc->side = 1;
	}
}

void	check_door_hit(t_game *game, t_raycast *rc)
{
	t_door	*door;
	int		orientation;

	door = find_door_at(game, rc->map_x, rc->map_y);
	if (!door || door->progress >= 0.99)
		return ;
	orientation = get_door_orientation(game, rc);
	if (orientation == 0)
		check_vertical_door(game, rc, door);
	else
		check_horizontal_door(game, rc, door);
}
