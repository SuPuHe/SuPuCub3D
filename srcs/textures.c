/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:03:36 by vpushkar          #+#    #+#             */
/*   Updated: 2025/11/11 13:00:43 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Loads additional wall textures from predefined file paths.
 *
 * Allocates memory for all wall textures and loads each one using
 * the MiniLibX `mlx_load_png()` function. If any texture fails to load,
 * an error message is displayed and the game exits.
 *
 * @param game Pointer to the main game structure containing texture data.
 *
 * @note This function assumes that `game->textures.wall_tex_paths` and
 *       `game->textures.wall_tex_count` have already been initialized.
 */
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

/**
 * @brief Initializes file paths for extra wall textures.
 *
 * Allocates memory for wall texture paths and fills them with
 * hardcoded texture file locations. The number of wall textures
 * is set to 8, and the corresponding file paths are stored in
 * `game->textures.wall_tex_paths`.
 *
 * @param game Pointer to the main game structure.
 *
 * @note The function uses `ft_strdup()` to duplicate the path strings,
 *       which should be freed later to avoid memory leaks.
 */
static void	more_walls_paths(t_game *game)
{
	game->textures.wall_tex_count = 8;
	game->textures.wall_tex_paths = malloc(sizeof(char *) * 8);
	game->textures.wall_tex_paths[0] = ft_strdup("textures/SUPPORT_4A.png");
	game->textures.wall_tex_paths[1] = ft_strdup("textures/SUPPORT_3A.PNG");
	game->textures.wall_tex_paths[2] = ft_strdup("textures/TECH_1C.png");
	game->textures.wall_tex_paths[3] = ft_strdup("textures/TECH_1E.png");
	game->textures.wall_tex_paths[4] = ft_strdup("textures/TECH_2F.png");
	game->textures.wall_tex_paths[5] = ft_strdup("textures/TECH_3B.png");
	game->textures.wall_tex_paths[6] = ft_strdup("textures/TECH_4E.png");
	game->textures.wall_tex_paths[7] = ft_strdup("textures/CONSOLE_1B.png");
	game->textures.wall_textures = NULL;
}

/**
 * @brief Loads all required game textures including walls, doors, and GUI.
 *
 * Loads the main directional wall textures (north, south, east, west),
 * as well as the door and GUI textures. Then calls helper functions
 * to set up and load additional wall textures from predefined paths.
 *
 * @note If any texture fails to load, an error is printed and the
 *       program exits via `exit_game(1)`.
 */
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
