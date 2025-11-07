/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 14:30:06 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	calculate_weapon_scale(t_game *game)
{
	int		frame_width;
	int		frame_height;
	float	screen_scale;

	frame_width = game->weapon.frames_right->width
		/ game->weapon.frame_count_right;
	frame_height = game->weapon.frames_right->height;
	screen_scale = (float)SCREEN_WIDTH / 1920.0f;
	game->weapon.scale = 0.5f * screen_scale;
	game->weapon.scaled_width = (int)(frame_width * game->weapon.scale);
	game->weapon.scaled_height = (int)(frame_height * game->weapon.scale);
}

void	get_frame_offsets(t_game *game, int *src_x_right, int *src_x_left)
{
	int	frame_width;
	int	left_offset;

	frame_width = game->weapon.frames_right->width
		/ game->weapon.frame_count_right;
	left_offset = 90;
	if (game->weapon.state == WEAPON_SHOOTING)
	{
		if (game->weapon.active_side == WEAPON_RIGHT)
		{
			*src_x_right = game->weapon.current_frame * frame_width;
			*src_x_left = left_offset;
		}
		else
		{
			*src_x_right = 0;
			*src_x_left = game->weapon.current_frame * frame_width
				+ left_offset;
		}
	}
	else
	{
		*src_x_right = 0;
		*src_x_left = left_offset;
	}
}

void	setup_weapon_positions(t_game *game, int *weapon_x, int *weapon_y)
{
	float	screen_scale;

	screen_scale = (float)SCREEN_WIDTH / 1920.0f;
	weapon_x[0] = SCREEN_WIDTH / 2 + (int)(100 * screen_scale);
	weapon_x[1] = SCREEN_WIDTH / 2 - game->weapon.scaled_width
		- (int)(100 * screen_scale);
	*weapon_y = SCREEN_HEIGHT - (int)(game->weapon.scaled_height * 0.75f)
		+ (int)game->weapon.bob_offset;
}
