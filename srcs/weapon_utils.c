/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/07 17:41:09 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Calculates weapon scaling based on screen resolution
 *
 * Computes adaptive weapon scale factor relative to 1920x1080 reference
 * resolution. Extracts individual frame dimensions from spritesheet and
 * applies base scale (0.5) multiplied by screen scale. Stores calculated
 * values in game structure for use in rendering pipeline.
 *
 * @param game Pointer to game structure containing weapon and screen data
 */
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

/**
 * @brief Calculates source texture offsets for current animation frame
 *
 * Determines X-coordinate offsets in weapon spritesheets based on current
 * animation state and active hand. During shooting, calculates frame offset
 * for active hand while keeping idle hand at frame 0. Left hand has 90px
 * offset to avoid sprite bleeding from adjacent frames. When idle, both
 * hands display frame 0.
 *
 * @param game Pointer to game structure with weapon state
 * @param src_x_right Pointer to store right hand X offset
 * @param src_x_left Pointer to store left hand X offset
 */
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

/**
 * @brief Calculates screen positions for both weapon hands
 *
 * Computes X and Y coordinates for weapon rendering with adaptive scaling.
 * Right hand positioned at screen center + offset, left hand at center
 * - weapon width - offset. Y position places weapon 75% visible with 25%
 * below screen edge, plus bobbing offset for movement effect. Positions
 * scale proportionally with screen resolution.
 *
 * @param game Pointer to game structure with weapon and screen data
 * @param weapon_x Array to store X positions [right=0, left=1]
 * @param weapon_y Pointer to store Y position (shared by both hands)
 */
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
