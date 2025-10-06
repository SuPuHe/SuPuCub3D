/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:55:27 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/06 12:46:49 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	handle_input(mlx_key_data_t keydata, void *params)
{
	t_game	*game;

	(void)params;
	game = ft_game();
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

void	player_move(void *param)
{
	t_game	*game;
	double	new_x;
	double	new_y;

	game = ft_game();
	(void)param;
	//game = (t_game *)param;
	new_x = game->player.x;
	new_y = game->player.y;
	if (game->player.move.forward)
	{
		new_x += game->player.dir_x * game->player.move_speed;
		new_y -= game->player.dir_y * game->player.move_speed;
	}
	if (game->player.move.backward)
	{
		new_x -= game->player.dir_x * game->player.move_speed;
		new_y += game->player.dir_y * game->player.move_speed;
	}
	if (game->player.move.turn_left)
		rotate_player(game, -game->player.rot_speed);
	if (game->player.move.turn_right)
		rotate_player(game, game->player.rot_speed);
	//printf("%c\n", game->map.grid[(int)new_y][(int)new_x]);
	if (game->map.grid[(int)new_y][(int)new_x] != '1')
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
	draw_player(game);
	// printf("Player: x=%.2f y=%.2f\n", game->player.x, game->player.y);
	// printf("Player: x=%.2f y=%.2f dir=(%.2f, %.2f)\n",
	// 	game->player.x, game->player.y,
	// 	game->player.dir_x, game->player.dir_y);
}

