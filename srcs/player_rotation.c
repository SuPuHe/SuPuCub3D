/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:40:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 16:47:02 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = game->player.dir_x * cos(angle)
		- game->player.dir_y * sin(angle);
	game->player.dir_y = old_dir_x * sin(angle)
		+ game->player.dir_y * cos(angle);
	game->player.plane_x = game->player.plane_x * cos(angle)
		- game->player.plane_y * sin(angle);
	game->player.plane_y = old_plane_x * sin(angle)
		+ game->player.plane_y * cos(angle);
}

static void	rotate_player_with_mouse(t_game *game, int delta_x, int center_x)
{
	double	rotation_angle;

	if (delta_x != 0)
	{
		rotation_angle = delta_x * game->player.mouse_sensitivity;
		rotate_player(game, rotation_angle);
	}
	mlx_set_mouse_pos(game->mlx, center_x, game->mlx->height / 2);
	game->player.last_mouse_x = center_x;
}

void	handle_mouse(void *param)
{
	t_game	*game;
	int		mouse_x;
	int		mouse_y;
	int		center_x;
	int		delta_x;

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
	rotate_player_with_mouse(game, delta_x, center_x);
}
