/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:00 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/04 16:52:39 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_weapon(t_game *game)
{
	// Load spritesheets (right and left hand weapons)
	game->weapon.frame_count_right = 5;
	game->weapon.frames_right = malloc(sizeof(mlx_texture_t *) * 1);
	game->weapon.frames_right[0] = mlx_load_png("textures/right_gun.png");
	game->weapon.frame_count_left = 5;
	game->weapon.frames_left = malloc(sizeof(mlx_texture_t *) * 1);
	game->weapon.frames_left[0] = mlx_load_png("textures/left_gun.png");
	// Check if loading was successful
	if (!game->weapon.frames_right[0])
		return ;
	if (!game->weapon.frames_left[0])
		return ;
	// Initialize weapon state
	game->weapon.current_frame = 0;
	game->weapon.frame_delay = 5;
	game->weapon.frame_timer = 0;
	game->weapon.state = WEAPON_IDLE;
	game->weapon.active_side = WEAPON_RIGHT;
	game->weapon.bob_offset = 0.0;
	game->weapon.bob_timer = 0.0;
	game->weapon.img_right = NULL;
	game->weapon.img_left = NULL;
}
void	update_weapon_bobbing(t_game *game)
{
	bool	is_moving;
	double	bob_speed;
	double	bob_amount;

	is_moving = game->player.move.forward || game->player.move.backward
		|| game->player.move.left || game->player.move.right;
	if (is_moving)
	{
		bob_speed = 0.15;
		bob_amount = 10.0;  // Bobbing amplitude in pixels
		game->weapon.bob_timer += bob_speed;
		game->weapon.bob_offset = sin(game->weapon.bob_timer) * bob_amount;
	}
	else
	{
		// Smoothly return weapon to center
		game->weapon.bob_offset *= 0.9;
		if (fabs(game->weapon.bob_offset) < 0.1)
			game->weapon.bob_offset = 0.0;
	}
}

void	update_weapon_animation(t_game *game)
{
	int	frame_count;

	game->weapon.frame_timer++;
	if (game->weapon.state == WEAPON_SHOOTING)
	{
		if (game->weapon.active_side == WEAPON_RIGHT)
			frame_count = game->weapon.frame_count_right;
		else
			frame_count = game->weapon.frame_count_left;
		if (game->weapon.frame_timer >= game->weapon.frame_delay)
		{
			game->weapon.frame_timer = 0;
			game->weapon.current_frame++;
			// Finished shooting animation
			if (game->weapon.current_frame >= frame_count)
			{
				game->weapon.current_frame = 0;
				game->weapon.state = WEAPON_IDLE;
				// Switch to the other hand for next shot
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

void	draw_weapon(void *param)
{
	mlx_texture_t	*spritesheet_right;
	mlx_texture_t	*spritesheet_left;
	int				weapon_x_right;
	int				weapon_x_left;
	int				weapon_y;
	t_game			*game;
	int				frame_width;
	int				frame_height;
	int				scaled_width;
	int				scaled_height;
	int				src_x_right;
	int				src_x_left;
	int				x, y, src_idx, dst_idx;
	float			scale;
	int				src_x, src_y;

	game = (t_game *)param;

	// Check data validity
	if (!game->weapon.frames_right || !game->weapon.frames_left)
		return ;
	if (!game->weapon.frames_right[0] || !game->weapon.frames_left[0])
		return ;
	spritesheet_right = game->weapon.frames_right[0];
	spritesheet_left = game->weapon.frames_left[0];
	// Calculate frame size (spritesheet divided into 5 frames)
	frame_width = spritesheet_right->width / game->weapon.frame_count_right;
	frame_height = spritesheet_right->height;
	// Scale weapon (90% of original size)
	scale = 0.9;
	scaled_width = (int)(frame_width * scale);
	scaled_height = (int)(frame_height * scale);
	// Calculate current frame offset in spritesheet
	// When shooting, animation applies only to active hand
	// Inactive hand shows frame 0 (idle)
	if (game->weapon.state == WEAPON_SHOOTING)
	{
		if (game->weapon.active_side == WEAPON_RIGHT)
		{
			src_x_right = game->weapon.current_frame * frame_width;
			src_x_left = 0;  // Left hand in idle
		}
		else
		{
			src_x_right = 0;  // Right hand in idle
			src_x_left = game->weapon.current_frame * frame_width;
		}
	}
	else
	{
		// In idle both hands show frame 0
		src_x_right = 0;
		src_x_left = 0;
	}
	// Position weapons (with offset to lower them)
	weapon_x_right = SCREEN_WIDTH / 2 + 100;
	weapon_x_left = SCREEN_WIDTH / 2 - scaled_width - 100;
	weapon_y = SCREEN_HEIGHT - scaled_height + 400 + (int)game->weapon.bob_offset;
	// Create image for right hand if it doesn't exist
	if (!game->weapon.img_right)
	{
		game->weapon.img_right = mlx_new_image(game->mlx, scaled_width, scaled_height);
		if (!game->weapon.img_right)
			return ;
		if (!mlx_image_to_window(game->mlx, game->weapon.img_right, weapon_x_right, weapon_y))
			return ;
		mlx_set_instance_depth(&game->weapon.img_right->instances[0], 10000);
		game->weapon.img_right->enabled = true;
	}
	// Create image for left hand if it doesn't exist
	if (!game->weapon.img_left)
	{
		game->weapon.img_left = mlx_new_image(game->mlx, scaled_width, scaled_height);
		if (!game->weapon.img_left)
			return ;
		if (!mlx_image_to_window(game->mlx, game->weapon.img_left, weapon_x_left, weapon_y))
			return ;
		mlx_set_instance_depth(&game->weapon.img_left->instances[0], 10000);
		game->weapon.img_left->enabled = true;
	}
	// Copy current frame from right hand spritesheet with scaling
	y = 0;
	while (y < scaled_height)
	{
		x = 0;
		while (x < scaled_width)
		{
			// Calculate corresponding position in source texture
			src_x = (int)(x / scale);
			src_y = (int)(y / scale);
			src_idx = ((src_y * spritesheet_right->width) + (src_x_right + src_x)) * 4;
			dst_idx = (y * scaled_width + x) * 4;
			game->weapon.img_right->pixels[dst_idx + 0] = spritesheet_right->pixels[src_idx + 0];
			game->weapon.img_right->pixels[dst_idx + 1] = spritesheet_right->pixels[src_idx + 1];
			game->weapon.img_right->pixels[dst_idx + 2] = spritesheet_right->pixels[src_idx + 2];
			game->weapon.img_right->pixels[dst_idx + 3] = spritesheet_right->pixels[src_idx + 3];
			x++;
		}
		y++;
	}
	// Copy current frame from left hand spritesheet with scaling
	y = 0;
	while (y < scaled_height)
	{
		x = 0;
		while (x < scaled_width)
		{
			// Calculate corresponding position in source texture
			src_x = (int)(x / scale);
			src_y = (int)(y / scale);
			src_idx = ((src_y * spritesheet_left->width) + (src_x_left + src_x)) * 4;
			dst_idx = (y * scaled_width + x) * 4;
			game->weapon.img_left->pixels[dst_idx + 0] = spritesheet_left->pixels[src_idx + 0];
			game->weapon.img_left->pixels[dst_idx + 1] = spritesheet_left->pixels[src_idx + 1];
			game->weapon.img_left->pixels[dst_idx + 2] = spritesheet_left->pixels[src_idx + 2];
			game->weapon.img_left->pixels[dst_idx + 3] = spritesheet_left->pixels[src_idx + 3];
			x++;
		}
		y++;
	}
	// Update positions (for bobbing effect)
	game->weapon.img_right->instances[0].x = weapon_x_right;
	game->weapon.img_right->instances[0].y = weapon_y;
	game->weapon.img_left->instances[0].x = weapon_x_left;
	game->weapon.img_left->instances[0].y = weapon_y;
	// Both hands always visible
	game->weapon.img_right->enabled = true;
	game->weapon.img_left->enabled = true;
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

// void	free_weapon(t_game *game)
// {
// 	if (game->weapon.frames_right && game->weapon.frames_right[0])
// 		mlx_delete_texture(game->weapon.frames_right[0]);
// 	if (game->weapon.frames_right)
// 		free(game->weapon.frames_right);

// 	if (game->weapon.frames_left && game->weapon.frames_left[0])
// 		mlx_delete_texture(game->weapon.frames_left[0]);
// 	if (game->weapon.frames_left)
// 		free(game->weapon.frames_left);

// 	if (game->weapon.img_right)
// 		mlx_delete_image(game->mlx, game->weapon.img_right);
// 	if (game->weapon.img_left)
// 		mlx_delete_image(game->mlx, game->weapon.img_left);
// }
