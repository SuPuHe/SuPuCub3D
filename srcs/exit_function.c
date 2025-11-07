/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:20:14 by omizin            #+#    #+#             */
/*   Updated: 2025/11/07 16:22:43 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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

void	exit_game(int exit_status)
{
	t_game	*game;

	game = ft_game();
	free_textures(game->textures);
	if (game->map.grid)
		free_split(game->map.grid);
	free_textures_path(game->textures);
	delete_weapon(game);
	if (game->mlx)
		mlx_terminate(game->mlx);
	exit(exit_status);
}
