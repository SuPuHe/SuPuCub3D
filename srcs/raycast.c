/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:05:22 by omizin            #+#    #+#             */
/*   Updated: 2025/10/09 13:52:22 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	init_ray(t_game *game, t_raycast *rc, int x)
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
}

static void	calculate_step_and_side_dist(t_game *game, t_raycast *rc)
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

static void	perform_dda(t_game *game, t_raycast *rc)
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
		if (game->map.grid[rc->map_y][rc->map_x] == '1')
			rc->hit = 1;
	}
}

static void	calculate_wall_distance(t_game *game, t_raycast *rc)
{
	if (rc->side == 0)
		rc->perp_wall_dist = (rc->map_x - game->player.x + (1 - rc->step_x) / 2.0) / rc->ray_dir_x;
	else
		rc->perp_wall_dist = (rc->map_y - game->player.y + (1 - rc->step_y) / 2.0) / rc->ray_dir_y;
}

static void	draw_column(t_game *game, int x, t_raycast *rc)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	uint32_t	color;
	int			y;

	line_height = (int)(SCREEN_HEIGHT / rc->perp_wall_dist);
	draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end >= SCREEN_HEIGHT)
		draw_end = SCREEN_HEIGHT - 1;
	if (rc->side == 0)
		color = 0xFF0000FF;
	else
		color = 0x00FF00FF;
	y = 0;
	while (y < SCREEN_HEIGHT)
	{
		if (y < draw_start)
			mlx_put_pixel(game->win_img, x, y, game->textures.ceil);
		else if (y >= draw_start && y <= draw_end)
			mlx_put_pixel(game->win_img, x, y, color);
		else
			mlx_put_pixel(game->win_img, x, y, game->textures.floor);
		y++;
	}
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
