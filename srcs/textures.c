/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:03:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/10 11:15:12 by omizin           ###   ########.fr       */
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
			print_error("Failed to load wall texture");
			exit_game(1);
		}
		i++;
	}
}

static void	more_walls_paths(t_game *game)
{
	game->textures.wall_tex_count = 8;
	game->textures.wall_tex_paths = malloc(sizeof(char *) * 8);
	game->textures.wall_tex_paths[0] = ft_strdup("textures/LAB_2B.png");
	game->textures.wall_tex_paths[1] = ft_strdup("textures/SUPPORT_3A.PNG");
	game->textures.wall_tex_paths[2] = ft_strdup("textures/TECH_1C.png");
	game->textures.wall_tex_paths[3] = ft_strdup("textures/TECH_1E.png");
	game->textures.wall_tex_paths[4] = ft_strdup("textures/TECH_2F.png");
	game->textures.wall_tex_paths[5] = ft_strdup("textures/TECH_3B.png");
	game->textures.wall_tex_paths[6] = ft_strdup("textures/TECH_4E.png");
	game->textures.wall_tex_paths[7] = ft_strdup("textures/CONSOLE_1B.png");
	game->textures.wall_textures = NULL;
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
		print_error("Faild to load textures.");
		exit_game(1);
	}
	more_walls_paths(game);
	more_walls_textures_load(game);
}
