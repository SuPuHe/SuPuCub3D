/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:05:22 by omizin            #+#    #+#             */
/*   Updated: 2025/11/07 11:28:09 by omizin           ###   ########.fr       */
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

static int	get_door_orientation(t_game *game, t_raycast *rc)
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

static void	check_vertical_door(t_game *game, t_raycast *rc, t_door *door)
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

static void	check_horizontal_door(t_game *game, t_raycast *rc, t_door *door)
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

static void	check_door_hit(t_game *game, t_raycast *rc)
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
	// Don't recalculate if it's a door (already calculated in perform_dda)
	if (rc->is_door)
		return ;
	if (rc->side == 0)
		rc->perp_wall_dist = (rc->map_x - game->player.x
				+ (1 - rc->step_x) / 2.0) / rc->ray_dir_x;
	else
		rc->perp_wall_dist = (rc->map_y - game->player.y
				+ (1 - rc->step_y) / 2.0) / rc->ray_dir_y;
}

static void	draw_pixel_from_texture(t_column_vars *c_vars, t_game *game)
{
	int		pixel_index;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	pixel_index = (c_vars->tex_y * c_vars->texture->width + c_vars->tex_x) * 4;
	// Check pixel_index bounds
	if (pixel_index >= 0 && pixel_index + 3
		< (int)(c_vars->texture->width * c_vars->texture->height * 4))
	{
		r = c_vars->texture->pixels[pixel_index + 0];
		g = c_vars->texture->pixels[pixel_index + 1];
		b = c_vars->texture->pixels[pixel_index + 2];
		a = c_vars->texture->pixels[pixel_index + 3];
		c_vars->color = (r << 24) | (g << 16) | (b << 8) | a;
		mlx_put_pixel(game->win_img, c_vars->x, c_vars->y, c_vars->color);
	}
}

static void	draw_column_loop(t_game *game, t_column_vars *c_vars)
{
	c_vars->y = 0;
	while (c_vars->y < SCREEN_HEIGHT)
	{
		if (c_vars->y < c_vars->draw_start)
			mlx_put_pixel(game->win_img, c_vars->x,
				c_vars->y, game->textures.ceil);
		else if (c_vars->y >= c_vars->draw_start
			&& c_vars->y <= c_vars->draw_end)
		{
			c_vars->tex_y = (int)c_vars->tex_pos
				& (c_vars->texture->height - 1);
			c_vars->tex_pos += c_vars->step;
			// Bounds check for pixel access
			if (c_vars->tex_x >= 0 && c_vars->tex_x
				< (int)c_vars->texture->width && c_vars->tex_y >= 0
				&& c_vars->tex_y < (int)c_vars->texture->height)
				draw_pixel_from_texture(c_vars, game);
		}
		else
			mlx_put_pixel(game->win_img, c_vars->x,
				c_vars->y, game->textures.floor);
		c_vars->y++;
	}
}

static void	choose_wall_texture(t_column_vars *c_vars,
	t_game *game, t_raycast *rc)
{
	c_vars->wall_char = game->map.grid[rc->map_y][rc->map_x];
	if (rc->is_door)
		c_vars->texture = game->textures.door_tex;
	else if (c_vars->wall_char >= '2' && c_vars->wall_char <= '9')
	{
		// Using additional textures for walls 2-9
		c_vars->wall_type = c_vars->wall_char - '2';// '2' -> 0, '3' -> 1, etc.
		if (c_vars->wall_type < game->textures.wall_tex_count)
			c_vars->texture = game->textures.wall_textures[c_vars->wall_type];
		else
			c_vars->texture = game->textures.north_tex;// Fallback
	}
	else if (rc->side == 0 && rc->ray_dir_x > 0)
		c_vars->texture = game->textures.east_tex;
	else if (rc->side == 0 && rc->ray_dir_x < 0)
		c_vars->texture = game->textures.west_tex;
	else if (rc->side == 1 && rc->ray_dir_y > 0)
		c_vars->texture = game->textures.south_tex;
	else
		c_vars->texture = game->textures.north_tex;
}

static void	check_if_door_and_bounds(t_column_vars *c_vars,
	t_game *game, t_raycast *rc)
{
	t_door	*door;

	if (rc->is_door)
	{
		door = find_door_at(game, rc->map_x, rc->map_y);
		if (door)
		{
			// Sliding effect: texture slides into the wall
			// As door opens (progress 0 -> 1), texture shifts
			// This creates the visual effect of door sliding sideways into pocket
			c_vars->wall_x += door->progress;
			// If texture has scrolled off screen, don't render
			if (c_vars->wall_x < 0.0 || c_vars->wall_x >= 1.0)
				return ;
		}
	}
	c_vars->tex_x = (int)(c_vars->wall_x * (double)c_vars->texture->width);
	// Bounds check for tex_x
	if (c_vars->tex_x < 0)
		c_vars->tex_x = 0;
	if (c_vars->tex_x >= (int)c_vars->texture->width)
		c_vars->tex_x = c_vars->texture->width - 1;
	// Vertical texture step
	c_vars->step = (double)c_vars->texture->height / c_vars->line_height;
	c_vars->tex_pos = (c_vars->draw_start - SCREEN_HEIGHT
			/ 2 + c_vars->line_height / 2) * c_vars->step;
}

static void	draw_column(t_game *game, int x, t_raycast *rc)
{
	t_column_vars	c_vars;

	c_vars.x = x;
	// Calculate wall height
	c_vars.line_height = (int)(SCREEN_HEIGHT / rc->perp_wall_dist);
	c_vars.draw_start = -c_vars.line_height / 2 + SCREEN_HEIGHT / 2;
	if (c_vars.draw_start < 0)
		c_vars.draw_start = 0;
	c_vars.draw_end = c_vars.line_height / 2 + SCREEN_HEIGHT / 2;
	if (c_vars.draw_end >= SCREEN_HEIGHT)
		c_vars.draw_end = SCREEN_HEIGHT - 1;
	// Choose texture based on wall type
	choose_wall_texture(&c_vars, game, rc);
	// Null check
	if (!c_vars.texture || !c_vars.texture->pixels)
		return ;
	// Calculate texture X coordinate
	if (rc->side == 0)
		c_vars.wall_x = game->player.y + rc->perp_wall_dist * rc->ray_dir_y;
	else
		c_vars.wall_x = game->player.x + rc->perp_wall_dist * rc->ray_dir_x;
	c_vars.wall_x -= floor(c_vars.wall_x);
	// For doors: adjust texture mapping for sliding effect
	check_if_door_and_bounds(&c_vars, game, rc);
	// Draw column
	draw_column_loop(game, &c_vars);
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
