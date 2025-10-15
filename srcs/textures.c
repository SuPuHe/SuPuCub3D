/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:03:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/10/13 16:41:42 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	textures_load(void)
{
	t_game	*game;

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
