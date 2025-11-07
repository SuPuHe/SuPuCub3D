/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:05:22 by omizin            #+#    #+#             */
/*   Updated: 2025/11/07 16:32:42 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
