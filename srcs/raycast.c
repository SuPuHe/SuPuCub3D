/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:05:22 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 12:32:02 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes raycasting parameters for a single screen column.
 *
 * Calculates the ray direction for the given column index `x`, determines
 * the starting map cell of the ray, and initializes delta distances used
 * in the DDA (Digital Differential Analyzer) algorithm.
 *
 * @param game Pointer to the main game structure.
 * @param rc Pointer to a raycasting structure where data will be stored.
 * @param x The current column index on the screen.
 *
 * @note The `camera_x` variable defines the ray direction relative
 *       to the player's camera plane, with values from -1 (left) to 1 (right).
 */
void	init_ray(t_game *game, t_raycast *rc, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0;
	rc->ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
	rc->ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;
	rc->map_x = (int)game->player.x;
	rc->map_y = (int)game->player.y;
	if (rc->ray_dir_x == 0)
		rc->delta_dist_x = 1e30;
	else
		rc->delta_dist_x = fabs(1 / rc->ray_dir_x);
	if (rc->ray_dir_y == 0)
		rc->delta_dist_y = 1e30;
	else
		rc->delta_dist_y = fabs(1 / rc->ray_dir_y);
	rc->hit = 0;
	rc->is_door = 0;
}

/**
 * @brief Calculates the step direction and initial side distances for the ray.
 *
 * Determines how the ray will move through the grid (left/right, up/down)
 * and computes the distance to the first x and y side intersections.
 * This data is required for the DDA algorithm to step through the map.
 *
 * @param game Pointer to the main game structure.
 * @param rc Pointer to the raycasting structure with initialized ray direction.
 */
void	calculate_step_and_side_dist(t_game *game, t_raycast *rc)
{
	if (rc->ray_dir_x < 0)
	{
		rc->step_x = -1;
		rc->side_dist_x = (game->player.x - rc->map_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0 - game->player.x) * rc->delta_dist_x;
	}
	if (rc->ray_dir_y < 0)
	{
		rc->step_y = -1;
		rc->side_dist_y = (game->player.y - rc->map_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0 - game->player.y) * rc->delta_dist_y;
	}
}

/**
 * @brief Executes the Digital Differential Analyzer (DDA) algorithm.
 *
 * Repeatedly advances the ray through the map grid until it hits a wall or door.
 * Determines whether the ray has encountered a solid block ('1'-'9')
 * or a door ('D').
 *
 * @param game Pointer to the main game structure.
 * @param rc Pointer to the raycasting structure containing ray state.
 *
 * @note When a door is detected, the function calls `check_door_hit()`
 * for additional handling.
 */
void	perform_dda(t_game *game, t_raycast *rc)
{
	while (rc->hit == 0)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += rc->step_x;
			rc->side = 0;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += rc->step_y;
			rc->side = 1;
		}
		if (game->map.grid[rc->map_y][rc->map_x] == '1'
			|| (game->map.grid[rc->map_y][rc->map_x] >= '2'
				&& game->map.grid[rc->map_y][rc->map_x] <= '9'))
		{
			rc->hit = 1;
			rc->is_door = 0;
		}
		else if (game->map.grid[rc->map_y][rc->map_x] == 'D')
			check_door_hit(game, rc);
	}
}

/**
 * @brief Calculates the perpendicular distance from the player to the wall hit.
 *
 * After the DDA algorithm determines where the ray hits a wall,
 * this function computes the perpendicular wall distance used for
 * correct perspective rendering (avoiding the fisheye effect).
 *
 * @param game Pointer to the main game structure.
 * @param rc Pointer to the raycasting structure containing hit data.
 */
void	calculate_wall_distance(t_game *game, t_raycast *rc)
{
	if (rc->is_door)
		return ;
	if (rc->side == 0)
		rc->perp_wall_dist = (rc->map_x - game->player.x
				+ (1 - rc->step_x) / 2.0) / rc->ray_dir_x;
	else
		rc->perp_wall_dist = (rc->map_y - game->player.y
				+ (1 - rc->step_y) / 2.0) / rc->ray_dir_y;
}

/**
 * @brief Renders the 3D view of the scene using raycasting.
 *
 * Iterates through each vertical stripe (screen column), casts a ray,
 * performs DDA to detect wall intersections, calculates distances, and
 * draws the corresponding textured column.
 *
 * @param game Pointer to the main game structure.
 *
 * @note This is the core function responsible for generating the 3D view
 *       from a 2D map using raycasting principles.
 */
void	render_3d_view(t_game *game)
{
	int			x;
	t_raycast	rc;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		init_ray(game, &rc, x);
		calculate_step_and_side_dist(game, &rc);
		perform_dda(game, &rc);
		calculate_wall_distance(game, &rc);
		draw_column(game, x, &rc);
		x++;
	}
}
