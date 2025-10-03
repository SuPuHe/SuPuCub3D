/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:55:27 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/03 17:21:28 by vpushkar         ###   ########.fr       */
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
		game->player.move.left = true;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE)
		game->player.move.left = false;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		game->player.move.right = true;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_RELEASE)
		game->player.move.right = false;
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
	// new_x = 0;
	// new_y = 0;
	if (game->player.move.forward)
	{
		//new_x += game->player.move_speed;
		new_y -=  game->player.move_speed;
	}
	if (game->player.move.backward)
	{
		//new_x -=  game->player.move_speed;
		new_y +=  game->player.move_speed;
	}
	if (game->player.move.left)
	{
		new_x -=  game->player.move_speed;
	//	new_y +=  game->player.move_speed;
	}
	if (game->player.move.right)
	{
		new_x +=  game->player.move_speed;
		//new_y -=  game->player.move_speed;
	}
	printf("%c\n", game->map.grid[(int)new_y][(int)new_x]);
	if (game->map.grid[(int)new_y][(int)new_x] != '1')
	{
		game->player.x = new_x;
		game->player.y = new_y;
	}
	printf("Player: x=%.2f y=%.2f\n", game->player.x, game->player.y);
}

