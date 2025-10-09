/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:05:22 by omizin            #+#    #+#             */
/*   Updated: 2025/10/09 12:05:28 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	render_3d_view(t_game *game)
{
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		// 1. Calculate ray position and direction
		double camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0; // -1 to 1
		double ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
		double ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;

		// 2. Map position
		int map_x = (int)game->player.x;
		int map_y = (int)game->player.y;

		// 3. Length of ray from current position to next x or y side
		double side_dist_x;
		double side_dist_y;

		// 4. Length of ray from one x or y side to next x or y side
		double delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
		double delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
		double perp_wall_dist;

		int step_x;
		int step_y;
		int hit = 0;
		int side;

		// 5. Calculate step and initial sideDist
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (game->player.x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - game->player.x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (game->player.y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - game->player.y) * delta_dist_y;
		}

		// 6. Perform DDA
		while (!hit)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0; // vertical wall
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1; // horizontal wall
			}
			if (game->map.grid[map_y][map_x] == '1')
				hit = 1;
		}

		// 7. Calculate distance to wall
		if (side == 0)
			perp_wall_dist = (map_x - game->player.x + (1 - step_x) / 2.0) / ray_dir_x;
		else
			perp_wall_dist = (map_y - game->player.y + (1 - step_y) / 2.0) / ray_dir_y;

		// 8. Calculate height of line to draw
		int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
		int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

		// 9. Choose wall color
		uint32_t color = (side == 0) ? 0xFF0000FF : 0x00FF00FF; // red for vertical, green for horizontal

		// 10. Draw vertical line
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			if (y < draw_start)
				mlx_put_pixel(game->win_img, x, y, game->textures.ceil); // ceiling gray
			else if (y >= draw_start && y <= draw_end)
				mlx_put_pixel(game->win_img, x, y, color);
			else
				mlx_put_pixel(game->win_img, x, y, game->textures.floor); // floor dark gray
		}
	}
}
