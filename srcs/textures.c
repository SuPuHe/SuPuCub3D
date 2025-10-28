/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:03:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/28 16:06:03 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	textures_load(void)
{
	t_game	*game;
	int		i;

	game = ft_game();
	game->textures.north_tex = mlx_load_png(game->textures.north_path);
	game->textures.south_tex = mlx_load_png(game->textures.south_path);
	game->textures.east_tex = mlx_load_png(game->textures.east_path);
	game->textures.west_tex = mlx_load_png(game->textures.west_path);
	game->textures.door_tex = mlx_load_png(game->textures.door_path);
	if (!game->textures.north_tex || !game->textures.south_tex
		|| !game->textures.east_tex || !game->textures.west_tex
		|| !game->textures.door_tex)
	{
		printf("ERROR: Faild to load textures.");
		exit(EXIT_FAILURE);
	}
	// Load additional textures for walls
	game->textures.wall_textures = malloc(sizeof(mlx_texture_t *) * game->textures.wall_tex_count);
	i = 0;
	while (i < game->textures.wall_tex_count)
	{
		game->textures.wall_textures[i] = mlx_load_png(game->textures.wall_tex_paths[i]);
		if (!game->textures.wall_textures[i])
		{
			printf("ERROR: Failed to load wall texture %d\n", i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

// void	image_create(void)
// {
// 	t_game	*game;

// 	game = ft_game();
// 	game->tx_images.north_img = mlx_texture_to_image(game->mlx, game->textures.north_tex);
// 	game->tx_images.south_img = mlx_texture_to_image(game->mlx, game->textures.south_tex);
// 	game->tx_images.east_img = mlx_texture_to_image(game->mlx, game->textures.east_tex);
// 	game->tx_images.west_img = mlx_texture_to_image(game->mlx, game->textures.west_tex);
// 	if (!game->tx_images.north_img || !game->tx_images.south_img
// 		|| !game->tx_images.east_img || !game->tx_images.west_img)
// 	{
// 		printf("ERROR: Faild to convert texutres to images.");
// 		exit(EXIT_FAILURE);
// 	}
// }
