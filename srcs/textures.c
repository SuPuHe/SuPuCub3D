/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:03:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/06 11:19:55 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	more_walls_textures_load(t_game *game)
{
	int	i;

	game->textures.wall_textures = malloc(sizeof(mlx_texture_t *)
			* game->textures.wall_tex_count);
	i = 0;
	while (i < game->textures.wall_tex_count)
	{
		game->textures.wall_textures[i]
			= mlx_load_png(game->textures.wall_tex_paths[i]);
		if (!game->textures.wall_textures[i])
		{
			printf("ERROR: Failed to load wall texture %d\n", i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	textures_load(void)
{
	t_game	*game;

	game = ft_game();
	game->textures.north_tex = mlx_load_png(game->textures.north_path);
	game->textures.south_tex = mlx_load_png(game->textures.south_path);
	game->textures.east_tex = mlx_load_png(game->textures.east_path);
	game->textures.west_tex = mlx_load_png(game->textures.west_path);
	game->textures.door_tex = mlx_load_png(game->textures.door_path);
	game->textures.gui_tex = mlx_load_png("./textures/billy_spritesheet.png");
	if (!game->textures.north_tex || !game->textures.south_tex
		|| !game->textures.east_tex || !game->textures.west_tex
		|| !game->textures.door_tex || !game->textures.gui_tex)
	{
		printf("ERROR: Faild to load textures.");
		exit(EXIT_FAILURE);
	}
	more_walls_textures_load(game);
}
