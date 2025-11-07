/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 16:32:40 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	draw_pixel_from_texture(t_column_vars *c_vars, t_game *game)
{
	int		pixel_index;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	pixel_index = (c_vars->tex_y * c_vars->texture->width + c_vars->tex_x) * 4;
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

void	draw_column_loop(t_game *game, t_column_vars *c_vars)
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

void	choose_wall_texture(t_column_vars *c_vars,
	t_game *game, t_raycast *rc)
{
	c_vars->wall_char = game->map.grid[rc->map_y][rc->map_x];
	if (rc->is_door)
		c_vars->texture = game->textures.door_tex;
	else if (c_vars->wall_char >= '2' && c_vars->wall_char <= '9')
	{
		c_vars->wall_type = c_vars->wall_char - '2';
		if (c_vars->wall_type < game->textures.wall_tex_count)
			c_vars->texture = game->textures.wall_textures[c_vars->wall_type];
		else
			c_vars->texture = game->textures.north_tex;
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

void	check_if_door_and_bounds(t_column_vars *c_vars,
	t_game *game, t_raycast *rc)
{
	t_door	*door;

	if (rc->is_door)
	{
		door = find_door_at(game, rc->map_x, rc->map_y);
		if (door)
		{
			c_vars->wall_x += door->progress;
			if (c_vars->wall_x < 0.0 || c_vars->wall_x >= 1.0)
				return ;
		}
	}
	c_vars->tex_x = (int)(c_vars->wall_x * (double)c_vars->texture->width);
	if (c_vars->tex_x < 0)
		c_vars->tex_x = 0;
	if (c_vars->tex_x >= (int)c_vars->texture->width)
		c_vars->tex_x = c_vars->texture->width - 1;
	c_vars->step = (double)c_vars->texture->height / c_vars->line_height;
	c_vars->tex_pos = (c_vars->draw_start - SCREEN_HEIGHT
			/ 2 + c_vars->line_height / 2) * c_vars->step;
}

void	draw_column(t_game *game, int x, t_raycast *rc)
{
	t_column_vars	c_vars;

	c_vars.x = x;
	c_vars.line_height = (int)(SCREEN_HEIGHT / rc->perp_wall_dist);
	c_vars.draw_start = -c_vars.line_height / 2 + SCREEN_HEIGHT / 2;
	if (c_vars.draw_start < 0)
		c_vars.draw_start = 0;
	c_vars.draw_end = c_vars.line_height / 2 + SCREEN_HEIGHT / 2;
	if (c_vars.draw_end >= SCREEN_HEIGHT)
		c_vars.draw_end = SCREEN_HEIGHT - 1;
	choose_wall_texture(&c_vars, game, rc);
	if (!c_vars.texture || !c_vars.texture->pixels)
		return ;
	if (rc->side == 0)
		c_vars.wall_x = game->player.y + rc->perp_wall_dist * rc->ray_dir_y;
	else
		c_vars.wall_x = game->player.x + rc->perp_wall_dist * rc->ray_dir_x;
	c_vars.wall_x -= floor(c_vars.wall_x);
	check_if_door_and_bounds(&c_vars, game, rc);
	draw_column_loop(game, &c_vars);
}
