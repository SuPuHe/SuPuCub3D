/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:01:05 by omizin            #+#    #+#             */
/*   Updated: 2025/11/11 13:04:11 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes default values for player and game structures.
 *
 * This function sets the initial movement speed, rotation speed,
 * collision radius, mouse sensitivity, and movement states of the player.
 * It also initializes the minimap and mouse settings.
 *
 * @param game Pointer to the main game structure.
 */
static void	structs_info_load(t_game *game)
{
	game->player.move_speed = 0.1;
	game->player.rot_speed = 0.05;
	game->player.collision_radius = 0.2;
	game->player.mouse_sensitivity = 0.001;
	game->player.last_mouse_x = -1;
	game->player.move.forward = false;
	game->player.move.backward = false;
	game->player.move.left = false;
	game->player.move.right = false;
	game->player.move.turn_left = false;
	game->player.move.turn_right = false;
	game->minimap.enabled = 0;
	game->mouse_enabled = 1;
}

/**
 * @brief Initializes the game environment and starts the main loop.
 *
 * This function performs the following:
 * - Initializes the MLX library and creates the game window.
 * - Loads textures.
 * - Creates the main rendering image.
 * - Calls structs_info_load to initialize player and game values.
 * - Sets mouse cursor mode.
 * - Initializes UI, doors, weapon, and minimap.
 * - Registers key and loop hooks for input, player movement,
 *		weapon updates, GUI animation, and mouse handling.
 * - Starts the MLX main loop.
 *
 * @param game Pointer to the main game structure.
 */
void	init_game(t_game *game)
{
	game->mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "Cub3D Test", true);
	if (!game->mlx)
		exit(1);
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	textures_load();
	game->win_img = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	mlx_image_to_window(game->mlx, game->win_img, 0, 0);
	structs_info_load(game);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
	init_ui();
	init_doors(game);
	init_weapon(game);
	init_minimap(game);
	mlx_key_hook(game->mlx, handle_input, NULL);
	mlx_loop_hook(game->mlx, handle_mouse, game);
	mlx_loop_hook(game->mlx, player_move, game);
	mlx_loop_hook(game->mlx, update_weapon, game);
	mlx_loop_hook(game->mlx, draw_weapon, game);
	mlx_loop_hook(game->mlx, animate_gui, game);
	mlx_loop(game->mlx);
}

/**
 * @brief Entry point of the program.
 *
 * This function checks command line arguments, parses the map file,
 * initializes the game, and starts the game loop.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Returns 0 on success, 1 on failure.
 */
int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
		return (print_error("Usage: ./cub3D maps/map.cub"), 1);
	game = ft_game();
	if (!parsing_file(argv[1]))
		exit_game(1);
	init_game(game);
	mlx_terminate(game->mlx);
	game->mlx = NULL;
	exit_game(0);
	return (0);
}
