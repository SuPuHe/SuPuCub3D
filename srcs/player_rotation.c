/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 14:40:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/10 12:53:20 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Rotates the player's direction and camera plane by a given angle.
 *
 * This function updates the player's facing direction (`dir_x`, `dir_y`) and
 * camera plane (`plane_x`, `plane_y`) using a standard 2D rotation matrix.
 * The rotation affects both the viewing direction and
 *  the rendered field of view.
 *
 * @param game Pointer to the main game structure containing player data.
 * @param angle The rotation angle in radians (positive for right
 * rotation, negative for left).
 */
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

/**
 * @brief Rotates the player based on horizontal mouse movement.
 *
 * Calculates the rotation angle from the mouse movement along the X-axis
 * and applies it to the player using `rotate_player()`. After rotation,
 * it resets the mouse position to the screen
 * center to maintain continuous movement.
 *
 * @param game Pointer to the main game structure.
 * @param delta_x The horizontal mouse movement distance from the center.
 * @param center_x The X-coordinate of the window's horizontal center.
 */
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

/**
 * @brief Handles player rotation using mouse input.
 *
 * This function retrieves the current mouse position and determines the
 * horizontal movement relative to the window center. The player is rotated
 * accordingly if mouse control is enabled. The cursor is kept centered for
 * consistent relative movement.
 *
 * @param param Unused parameter (kept for compatibility
 * with the MLX hook system).
 */
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
