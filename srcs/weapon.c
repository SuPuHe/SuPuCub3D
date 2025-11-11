/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/11 14:47:27 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes the weapon system
 *
 * Loads weapon sprite sheets for both right and left hands from PNG files.
 * Sets up initial weapon state including frame counts, animation parameters,
 * and bobbing effect values. Each weapon has 5 animation frames.
 *
 * @param game Pointer to the main game structure
 */
void	init_weapon(t_game *game)
{
	game->weapon.frame_count_right = 5;
	game->weapon.frames_right = mlx_load_png("textures/sprites/right_gun.png");
	game->weapon.frame_count_left = 5;
	game->weapon.frames_left = mlx_load_png("textures/sprites/left_gun.png");
	if (!game->weapon.frames_right)
		exit_game(1);
	if (!game->weapon.frames_left)
		exit_game(1);
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

/**
 * @brief Updates weapon bobbing effect during player movement
 *
 * Creates a realistic weapon bobbing animation using sine wave when the player
 * is moving forward or backward. The bobbing effect smoothly decays to zero
 * when the player stops moving. Amplitude scales with screen height.
 *
 * @param game Pointer to the main game structure
 */
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

/**
 * @brief Updates weapon shooting animation frames
 *
 * Manages the frame-by-frame progression of the weapon shooting animation.
 * Handles frame timing, counts frames for current animation cycle, and
 * switches between right and left hands after each complete animation.
 * Resets to idle state when animation completes.
 *
 * @param game Pointer to the main game structure
 */
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

/**
 * @brief Main weapon update function called each frame
 *
 * Orchestrates weapon system updates by calling both bobbing and animation
 * update functions. This function is registered as an MLX loop hook and
 * executes every frame to maintain smooth weapon animations.
 *
 * @param param Void pointer to the game structure (cast to t_game*)
 */
void	update_weapon(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	update_weapon_bobbing(game);
	update_weapon_animation(game);
}

/**
 * @brief Triggers weapon shooting animation
 *
 * Initiates the shooting animation sequence if the weapon is currently in
 * idle state. Resets frame counters and sets weapon state to shooting mode.
 * The actual frame-by-frame animation is handled by update_weapon_animation().
 *
 * @param game Pointer to the main game structure
 */
void	weapon_shoot(t_game *game)
{
	if (game->weapon.state == WEAPON_IDLE)
	{
		game->weapon.state = WEAPON_SHOOTING;
		game->weapon.current_frame = 0;
		game->weapon.frame_timer = 0;
	}
}
