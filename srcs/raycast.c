/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:05:22 by omizin            #+#    #+#             */
/*   Updated: 2025/10/24 15:02:19 by vpushkar         ###   ########.fr       */
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
	rc->is_door = 0;
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
	t_door	*door;
	double	door_pos;
	double	ray_pos;
	double	dist_to_center;
	int		door_orientation;

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
		{
			rc->hit = 1;
			rc->is_door = 0;
		}
		else if (game->map.grid[rc->map_y][rc->map_x] == 'D')
		{
			door = find_door_at(game, rc->map_x, rc->map_y);
			if (door && door->progress < 0.99)
			{
				// Determine door orientation by checking adjacent tiles
				// Door is PERPENDICULAR to the walls around it
				// 0 = vertical (N-S), 1 = horizontal (E-W)
				door_orientation = 0;

				// If walls on left/right -> door blocks N-S corridor -> door is E-W
				if ((rc->map_x > 0 && game->map.grid[rc->map_y][rc->map_x - 1] == '1') ||
					(rc->map_x < game->map.width - 1 && game->map.grid[rc->map_y][rc->map_x + 1] == '1'))
					door_orientation = 1;  // Horizontal door (E-W)
				// If walls on top/bottom -> door blocks E-W corridor -> door is N-S
				else if ((rc->map_y > 0 && game->map.grid[rc->map_y - 1][rc->map_x] == '1') ||
					(rc->map_y < game->map.height - 1 && game->map.grid[rc->map_y + 1][rc->map_x] == '1'))
					door_orientation = 0;  // Vertical door (N-S)

				// Calculate intersection with door plane at tile center
				if (door_orientation == 0)
				{
					// Vertical door at X = map_x + 0.5
					if (rc->ray_dir_x != 0)
					{
						dist_to_center = ((double)rc->map_x + 0.5 - game->player.x) / rc->ray_dir_x;

						// Door visible from both sides - just check if distance is positive
						if (dist_to_center > 0)
						{
							ray_pos = game->player.y + dist_to_center * rc->ray_dir_y;
							door_pos = ray_pos - (double)rc->map_y;

							if (door_pos >= 0.0 && door_pos <= 1.0 && door_pos <= (1.0 - door->progress))
							{
								rc->is_door = 1;
								rc->hit = 1;
								rc->perp_wall_dist = fabs(dist_to_center);
								rc->side = 0;
							}
						}
					}
				}
				else
				{
					// Horizontal door at Y = map_y + 0.5
					if (rc->ray_dir_y != 0)
					{
						dist_to_center = ((double)rc->map_y + 0.5 - game->player.y) / rc->ray_dir_y;

						// Door visible from both sides - just check if distance is positive
						if (dist_to_center > 0)
						{
							ray_pos = game->player.x + dist_to_center * rc->ray_dir_x;
							door_pos = ray_pos - (double)rc->map_x;

							if (door_pos >= 0.0 && door_pos <= 1.0 && door_pos <= (1.0 - door->progress))
							{
								rc->is_door = 1;
								rc->hit = 1;
								rc->perp_wall_dist = fabs(dist_to_center);
								rc->side = 1;
							}
						}
					}
				}
			}
		}
	}
}

static void	calculate_wall_distance(t_game *game, t_raycast *rc)
{
	// Don't recalculate if it's a door (already calculated in perform_dda)
	if (rc->is_door)
		return;

	if (rc->side == 0)
		rc->perp_wall_dist = (rc->map_x - game->player.x + (1 - rc->step_x) / 2.0) / rc->ray_dir_x;
	else
		rc->perp_wall_dist = (rc->map_y - game->player.y + (1 - rc->step_y) / 2.0) / rc->ray_dir_y;
}

static void	draw_column(t_game *game, int x, t_raycast *rc)
{
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				y;
	int				tex_x;
	int				tex_y;
	double			step;
	double			tex_pos;
	mlx_texture_t	*texture;
	uint32_t		color;
	double			wall_x;

	// Calculate wall height
	line_height = (int)(SCREEN_HEIGHT / rc->perp_wall_dist);
	draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end >= SCREEN_HEIGHT)
		draw_end = SCREEN_HEIGHT - 1;

	// Choose texture
	if (rc->is_door)
		texture = game->textures.door_tex;
	else if (rc->side == 0 && rc->ray_dir_x > 0)
		texture = game->textures.east_tex;
	else if (rc->side == 0 && rc->ray_dir_x < 0)
		texture = game->textures.west_tex;
	else if (rc->side == 1 && rc->ray_dir_y > 0)
		texture = game->textures.south_tex;
	else
		texture = game->textures.north_tex;

	// Null check
	if (!texture || !texture->pixels)
		return ;

	// Calculate texture X coordinate
	if (rc->side == 0)
		wall_x = game->player.y + rc->perp_wall_dist * rc->ray_dir_y;
	else
		wall_x = game->player.x + rc->perp_wall_dist * rc->ray_dir_x;
	wall_x -= floor(wall_x);

	// For doors: adjust texture mapping for sliding effect
	if (rc->is_door)
	{
		t_door *door = find_door_at(game, rc->map_x, rc->map_y);
		if (door)
		{
			// wall_x is already 0.0 to 1.0 from the door hit position
			// No need to adjust, texture already maps correctly
			// Just ensure it's in valid range
			if (wall_x < 0.0)
				wall_x = 0.0;
			if (wall_x >= 1.0)
				wall_x = 0.999;
		}
	}

	tex_x = (int)(wall_x * (double)texture->width);

	// Bounds check for tex_x
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= (int)texture->width)
		tex_x = texture->width - 1;

	// Vertical texture step
	step = (double)texture->height / line_height;
	tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

	// Draw column
	for (y = 0; y < SCREEN_HEIGHT; y++)
	{
		if (y < draw_start)
			mlx_put_pixel(game->win_img, x, y, game->textures.ceil);
		else if (y >= draw_start && y <= draw_end)
		{
			tex_y = (int)tex_pos & (texture->height - 1);
			tex_pos += step;

			// Bounds check for pixel access
			if (tex_x >= 0 && tex_x < (int)texture->width
				&& tex_y >= 0 && tex_y < (int)texture->height)
			{
				int pixel_index = (tex_y * texture->width + tex_x) * 4;

				// Check pixel_index bounds
				if (pixel_index >= 0 &&
					pixel_index + 3 < (int)(texture->width * texture->height * 4))
				{
					uint8_t r = texture->pixels[pixel_index + 0];
					uint8_t g = texture->pixels[pixel_index + 1];
					uint8_t b = texture->pixels[pixel_index + 2];
					uint8_t a = texture->pixels[pixel_index + 3];
					color = (r << 24) | (g << 16) | (b << 8) | a;
					mlx_put_pixel(game->win_img, x, y, color);
				}
			}
		}
		else
			mlx_put_pixel(game->win_img, x, y, game->textures.floor);
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
