/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_doors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/13 12:59:47 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Determines the orientation of a door on the map.
 *
 * This function checks surrounding wall tiles to determine if a door is
 * vertically or horizontally oriented. If there are walls ('1') on the left or
 * right of the door, it is considered vertical (returns 1). Otherwise, if there
 * are walls above or below, it is considered horizontal (returns 0).
 *
 * @param game Pointer to the main game structure containing the map grid.
 * @param rc Pointer to the current raycasting
 * structure containing hit coordinates.
 * @return int 1 if the door is vertical, 0 if horizontal or undefined.
 */
int	get_door_orientation(t_game *game, t_raycast *rc)
{
	if ((rc->map_x > 0
			&& ft_strchr(MAP_WALLS, game->map.grid[rc->map_y][rc->map_x - 1]))
			&& (rc->map_x < game->map.width - 1
			&& ft_strchr(MAP_WALLS, game->map.grid[rc->map_y][rc->map_x + 1])))
		return (1);
	if ((rc->map_y > 0
			&& ft_strchr(MAP_WALLS, game->map.grid[rc->map_y - 1][rc->map_x]))
			&& (rc->map_y < game->map.height - 1
			&& ft_strchr(MAP_WALLS, game->map.grid[rc->map_y - 1][rc->map_x])))
		return (0);
	return (0);
}

/**
 * @brief Checks if a ray intersects a vertically oriented door.
 *
 * This function calculates the intersection between
 * a ray and a vertical door line. If the intersection lies within
 * the door's boundaries and not beyond the closed part of the door
 * (based on door->progress), it sets the ray as hitting a door.
 *
 * @param game Pointer to the main game structure
 * containing player position data.
 * @param rc Pointer to the raycasting structure containing
 * ray direction and hit data.
 * @param door Pointer to the door structure representing the door being tested.
 */
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

/**
 * @brief Checks if a ray intersects a horizontally oriented door.
 *
 * Similar to check_vertical_door(), but used when the door is horizontal.
 * It calculates the intersection based on the Y component of the ray direction.
 * If the ray hits the solid part of the door, the
 * raycast data is updated accordingly.
 *
 * @param game Pointer to the main game structure
 * containing player position data.
 * @param rc Pointer to the raycasting structure containing
 * ray direction and hit data.
 * @param door Pointer to the door structure representing the door being tested.
 */
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

/**
 * @brief Determines whether the current ray has hit a door and updates ray data.
 *
 * Finds the door at the current map position (if any), checks whether it is open
 * enough to be ignored, and tests for intersection with either a vertical or
 * horizontal door using the appropriate helper function.
 *
 * @param game Pointer to the main game structure
 * containing the door list and map.
 * @param rc Pointer to the raycasting structure
 * that will be updated if a door is hit.
 */
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
