/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:55:27 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/04 14:59:38 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	handle_input(mlx_key_data_t keydata, void *params)
{
	t_game	*game;

	(void)params;
	game = ft_game();
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
		game->player.move.forward = true;
	if (keydata.key == MLX_KEY_W && keydata.action == MLX_RELEASE)
		game->player.move.forward = false;
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_PRESS)
		game->player.move.backward = true;
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_RELEASE)
		game->player.move.backward = false;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
		game->player.move.turn_left = true;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE)
		game->player.move.turn_left = false;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		game->player.move.turn_right = true;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_RELEASE)
		game->player.move.turn_right = false;
	if (keydata.key == MLX_KEY_E && keydata.action == MLX_PRESS)
		interact_with_door(game);
	if (keydata.key == MLX_KEY_M && keydata.action == MLX_PRESS)
		check_minimap(game);
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_PRESS)
		weapon_shoot(game);
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
	{
		game->mouse_enabled = !game->mouse_enabled;
		if (game->mouse_enabled)
		{
			mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
			game->player.last_mouse_x = -1;
		}
		else
			mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
	}
}

void	handle_mouse(void *param)
{
	t_game	*game;
	int		mouse_x;
	int		mouse_y;
	int		center_x;
	int		delta_x;
	double	rotation_angle;

	(void)param;
	game = ft_game();

	if (!game->mouse_enabled)
		return ;

	center_x = game->mlx->width / 2;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);

	if (game->player.last_mouse_x == -1)
	{
		game->player.last_mouse_x = center_x;
		mlx_set_mouse_pos(game->mlx, center_x, game->mlx->height / 2);
		return ;
	}

	delta_x = mouse_x - center_x;

	if (delta_x != 0)
	{
		rotation_angle = delta_x * game->player.mouse_sensitivity;
		rotate_player(game, rotation_angle);
	}

	// Return mouse cursor to the middle of the screen
	mlx_set_mouse_pos(game->mlx, center_x, game->mlx->height / 2);
	game->player.last_mouse_x = center_x;
}

void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	// direction change
	game->player.dir_x = game->player.dir_x * cos(angle)
		- game->player.dir_y * sin(angle);
	game->player.dir_y = old_dir_x * sin(angle)
		+ game->player.dir_y * cos(angle);
	// fov change
	game->player.plane_x = game->player.plane_x * cos(angle)
		- game->player.plane_y * sin(angle);
	game->player.plane_y = old_plane_x * sin(angle)
		+ game->player.plane_y * cos(angle);
}

static bool	is_valid_position(t_game *game, double x, double y)
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

static void	move_player_with_collision(t_game *game, double new_x, double new_y)
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

void	player_move(void *param)
{
	t_game	*game;
	double	new_x;
	double	new_y;
	double	move_dx;
	double	move_dy;

	game = ft_game();
	(void)param;
	new_x = game->player.x;
	new_y = game->player.y;
	move_dx = 0;
	move_dy = 0;
	if (game->player.move.forward)
	{
		move_dx += game->player.dir_x * game->player.move_speed;
		move_dy += game->player.dir_y * game->player.move_speed;
	}
	if (game->player.move.backward)
	{
		move_dx -= game->player.dir_x * game->player.move_speed;
		move_dy -= game->player.dir_y * game->player.move_speed;
	}
	if (game->player.move.turn_left)
		rotate_player(game, -game->player.rot_speed);
	if (game->player.move.turn_right)
		rotate_player(game, game->player.rot_speed);
	new_x += move_dx;
	new_y += move_dy;
	move_player_with_collision(game, new_x, new_y);
	update_doors(game);
	if (game->minimap.enabled)
		update_minimap(game);
	render_3d_view(game);
}

