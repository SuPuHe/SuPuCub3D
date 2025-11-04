/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:01:05 by omizin            #+#    #+#             */
/*   Updated: 2025/11/04 14:59:37 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_game(t_game *game)
{
	game->mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "Cub3D Test", true);
	if (!game->mlx)
		exit(1);

	mlx_set_setting(MLX_STRETCH_IMAGE, true);

	game->textures.wall_tex_count = 8;
	game->textures.wall_tex_paths = malloc(sizeof(char *) * 8);
	game->textures.wall_tex_paths[0] = ft_strdup("textures/LAB_2B.png");
	game->textures.wall_tex_paths[1] = ft_strdup("textures/SUPPORT_3A.PNG");
	game->textures.wall_tex_paths[2] = ft_strdup("textures/TECH_1C.png");
	game->textures.wall_tex_paths[3] = ft_strdup("textures/TECH_1E.png");
	game->textures.wall_tex_paths[4] = ft_strdup("textures/TECH_2F.png");
	game->textures.wall_tex_paths[5] = ft_strdup("textures/TECH_3B.png");
	game->textures.wall_tex_paths[6] = ft_strdup("textures/TECH_4E.png");
	// game->textures.wall_tex_paths[7] = ft_strdup("textures/TECH_4F.png");
	game->textures.wall_tex_paths[7] = ft_strdup("textures/CONSOLE_1B.png");


	game->textures.wall_textures = NULL;

	textures_load();
	//image_create();
	game->win_img = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	mlx_image_to_window(game->mlx, game->win_img, 0, 0);

	init_ui();

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
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
}

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
		return (print_error("Usage: ./cub maps/map.cub"), 1);
	game = ft_game();
	if (!parsing_file(argv[1]))
		return (free_textures_path(game->textures), free_split(game->map.grid), 1);
	init_game(game);
	init_doors(game);
	init_weapon(game);
	init_minimap(game);
	//draw_minimap(game);
	//draw_player(game);

	mlx_key_hook(game->mlx, handle_input, NULL);
	mlx_loop_hook(game->mlx, handle_mouse, game);
	mlx_loop_hook(game->mlx, player_move, game);
	mlx_loop_hook(game->mlx, update_weapon, game);
	mlx_loop_hook(game->mlx, draw_weapon, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
	free_textures_path(game->textures);
	free_split(game->map.grid);
	return (0);
}
