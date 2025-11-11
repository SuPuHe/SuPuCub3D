/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:17:44 by omizin            #+#    #+#             */
/*   Updated: 2025/11/11 11:37:03 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes the game's GUI (Heads-Up Display) frame.
 *
 * Creates a new image buffer for the GUI using the MiniLibX library
 * and places it on the game window. If image creation fails, an error
 * message is printed and the program sets the exit flag.
 *
 * @note The GUI frame is positioned horizontally centered near the bottom
 *       of the screen with a fixed size defined by FRAME_SIZE.
 */
void	init_ui(void)
{
	t_game	*game;

	game = ft_game();
	game->tx_images.gui_img = mlx_new_image(game->mlx, FRAME_SIZE, FRAME_SIZE);
	if (!game->tx_images.gui_img)
	{
		print_error("Failed to create GUI frame image");
		exit_game(1);
	}
	mlx_image_to_window(game->mlx, game->tx_images.gui_img,
		SCREEN_WIDTH / 2 - (235 / 2), SCREEN_HEIGHT - 250);
}

/**
 * @brief Draws a specific GUI frame from the sprite sheet onto the screen.
 *
 * Extracts one frame from the GUI texture atlas and copies its pixels
 * into the GUI image buffer for display. The frame is selected using
 * the `frame_index` parameter.
 *
 * @param game Pointer to the main game structure.
 * @param frame_index The index of the frame to draw from the GUI texture sheet.
 *
 * @note Each frame is assumed to have a fixed size (FRAME_SIZE × FRAME_SIZE)
 *       and the texture sheet is laid out horizontally.
 */
void	gui_set_frame(t_game *game, int frame_index)
{
	int			sheet_x;
	int			x;
	int			y;
	int			src;
	uint32_t	color;

	sheet_x = frame_index * FRAME_SIZE;
	y = 0;
	while (y < FRAME_SIZE)
	{
		x = 0;
		while (x < FRAME_SIZE)
		{
			src = ((y * 9165) + (sheet_x + x)) * 4;
			color = (game->textures.gui_tex->pixels[src + 0] << 24)
				| (game->textures.gui_tex->pixels[src + 1] << 16)
				| (game->textures.gui_tex->pixels[src + 2] << 8)
				| (game->textures.gui_tex->pixels[src + 3]);
			mlx_put_pixel(game->tx_images.gui_img, x, y, color);
			x++;
		}
		y++;
	}
}

/**
 * @brief Animates the GUI by cycling through frames over time.
 *
 * This function updates the GUI frame at regular intervals (every 0.1 seconds).
 * It should be called repeatedly, typically in the main game loop or as
 * a hooked update function in the MLX event system.
 *
 * @param params A pointer to the main game structure (`t_game *`).
 *
 * @note The animation cycles through 26 frames before restarting.
 */
void	animate_gui(void *params)
{
	static double	last_time = 0;
	static int		frame = 0;
	double			now;
	t_game			*game;

	game = (t_game *)params;
	now = mlx_get_time();
	if (now - last_time > 0.1)
	{
		gui_set_frame(game, frame);
		frame = (frame + 1) % 26;
		last_time = now;
	}
}
