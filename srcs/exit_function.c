/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:20:14 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:08:56 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Frees a NULL-terminated array of strings.
 *
 * Iterates through the array, freeing each string, then
 * frees the array itself.
 *
 * @param lines The array of strings to free.
 */
void	free_split(char **lines)
{
	int	i;

	i = 0;
	if (!lines)
		return ;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

/**
 * @brief Frees allocated file paths in the textures structure.
 *
 * Frees the paths for north, south, east, west, door textures,
 * and any additional wall texture paths.
 *
 * @param textures The textures structure containing paths to free.
 */
static void	free_textures_path(t_textures textures)
{
	int	i;

	i = 0;
	if (textures.north_path)
		free(textures.north_path);
	if (textures.south_path)
		free(textures.south_path);
	if (textures.east_path)
		free(textures.east_path);
	if (textures.west_path)
		free(textures.west_path);
	if (textures.door_path)
		free(textures.door_path);
	while (i < textures.wall_tex_count)
	{
		free(textures.wall_tex_paths[i]);
		i++;
	}
}

/**
 * @brief Deletes loaded textures from memory.
 *
 * Calls `mlx_delete_texture` on each loaded texture in the
 * textures structure, including wall textures and GUI textures.
 *
 * @param textures The textures structure containing textures to delete.
 */
static void	free_textures(t_textures textures)
{
	int	i;

	if (textures.north_tex)
		mlx_delete_texture(textures.north_tex);
	if (textures.south_tex)
		mlx_delete_texture(textures.south_tex);
	if (textures.east_tex)
		mlx_delete_texture(textures.east_tex);
	if (textures.west_tex)
		mlx_delete_texture(textures.west_tex);
	if (textures.door_tex)
		mlx_delete_texture(textures.door_tex);
	if (textures.gui_tex)
		mlx_delete_texture(textures.gui_tex);
	if (textures.wall_textures)
	{
		i = 0;
		while (i < textures.wall_tex_count)
		{
			if (textures.wall_textures[i])
				mlx_delete_texture(textures.wall_textures[i]);
			i++;
		}
		free(textures.wall_textures);
	}
}

/**
 * @brief Deletes the weapon textures from memory.
 *
 * Frees the left and right weapon frame textures if they exist.
 *
 * @param game Pointer to the game structure.
 */
static void	delete_weapon(t_game *game)
{
	if (game->weapon.frames_left)
	{
		mlx_delete_texture(game->weapon.frames_left);
	}
	if (game->weapon.frames_right)
	{
		mlx_delete_texture(game->weapon.frames_right);
	}
}

/**
 * @brief Cleans up all allocated resources and exits the game.
 *
 * Frees map grids, copied map, texture paths, loaded textures,
 * weapon textures, and terminates the MLX instance. Exits with
 * the given status.
 *
 * @param exit_status The exit code to terminate the program with.
 */
void	exit_game(int exit_status)
{
	t_game	*game;

	game = ft_game();
	free_textures(game->textures);
	if (game->map.grid)
		free_split(game->map.grid);
	if (game->map.copy_map)
		free_split(game->map.copy_map);
	free_textures_path(game->textures);
	delete_weapon(game);
	if (game->mlx)
		mlx_terminate(game->mlx);
	exit(exit_status);
}
