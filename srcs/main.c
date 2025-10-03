/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:01:05 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 18:59:39 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	init_game(t_game *game)
{
	game->mlx = mlx_init(800, 600, "Cub3D Test", true);
	if (!game->mlx)
		exit(1);

	game->player.dir_x = 1.0;
	game->player.dir_y = 0.0;
	game->player.move_speed = 0.1;
	game->player.rot_speed = 0.05;

	game->player.move.forward = false;
	game->player.move.backward = false;
	game->player.move.left = false;
	game->player.move.right = false;
	game->player.move.turn_left = false;
	game->player.move.turn_right = false;
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
	mlx_key_hook(game->mlx, handle_input, NULL);
	mlx_loop_hook(game->mlx, player_move, game);
	mlx_loop(game->mlx);
	mlx_terminate(game->mlx);
	free_textures_path(game->textures);
	free_split(game->map.grid);
	return (0);
}
