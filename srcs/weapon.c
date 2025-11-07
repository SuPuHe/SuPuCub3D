/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 14:29:48 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_weapon(t_game *game)
{
	game->weapon.frame_count_right = 5;
	game->weapon.frames_right = mlx_load_png("textures/sprites/right_gun.png");
	game->weapon.frame_count_left = 5;
	game->weapon.frames_left = mlx_load_png("textures/sprites/left_gun.png");
	if (!game->weapon.frames_right)
		return ;
	if (!game->weapon.frames_left)
		return ;
	game->weapon.current_frame = 0;
	game->weapon.last_drawn_frame_right = -1;
	game->weapon.last_drawn_frame_left = -1;
	game->weapon.frame_delay = 5;
	game->weapon.frame_timer = 0;
	game->weapon.state = WEAPON_IDLE;
	game->weapon.active_side = WEAPON_RIGHT;
	game->weapon.bob_offset = 0.0;
	game->weapon.bob_timer = 0.0;
	game->weapon.img_right = NULL;
	game->weapon.img_left = NULL;
}

static void	update_weapon_bobbing(t_game *game)
{
	bool	is_moving;
	double	bob_speed;
	double	bob_amount;
	float	screen_scale;

	if (game->player.move.forward || game->player.move.backward)
		is_moving = true;
	else
		is_moving = false;
	if (is_moving)
	{
		bob_speed = 0.15;
		screen_scale = (float)SCREEN_HEIGHT / 1080.0f;
		bob_amount = 10.0 * screen_scale;
		game->weapon.bob_timer += bob_speed;
		game->weapon.bob_offset = sin(game->weapon.bob_timer) * bob_amount;
	}
	else
	{
		game->weapon.bob_offset *= 0.9;
		if (fabs(game->weapon.bob_offset) < 0.1)
			game->weapon.bob_offset = 0.0;
	}
}

static void	update_weapon_animation(t_game *game)
{
	game->weapon.frame_timer++;
	if (game->weapon.state == WEAPON_SHOOTING)
	{
		if (game->weapon.active_side == WEAPON_RIGHT)
			game->weapon.frame_count = game->weapon.frame_count_right;
		else
			game->weapon.frame_count = game->weapon.frame_count_left;
		if (game->weapon.frame_timer >= game->weapon.frame_delay)
		{
			game->weapon.frame_timer = 0;
			game->weapon.current_frame++;
			if (game->weapon.current_frame >= game->weapon.frame_count)
			{
				game->weapon.current_frame = 0;
				game->weapon.state = WEAPON_IDLE;
				if (game->weapon.active_side == WEAPON_RIGHT)
					game->weapon.active_side = WEAPON_LEFT;
				else
					game->weapon.active_side = WEAPON_RIGHT;
			}
		}
	}
	else
		game->weapon.current_frame = 0;
}

void	update_weapon(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	update_weapon_bobbing(game);
	update_weapon_animation(game);
}

void	weapon_shoot(t_game *game)
{
	if (game->weapon.state == WEAPON_IDLE)
	{
		game->weapon.state = WEAPON_SHOOTING;
		game->weapon.current_frame = 0;
		game->weapon.frame_timer = 0;
	}
}
