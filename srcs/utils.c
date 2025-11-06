/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:16:29 by omizin            #+#    #+#             */
/*   Updated: 2025/11/06 11:23:02 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	print_error(char *msg)
{
	write(2, RED, 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n"RESET, 6);
	ft_game()->exit = 1;
}

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

void	free_textures_path(t_textures textures)
{
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
}

t_game	*ft_game(void)
{
	static t_game	game;

	return (&game);
}

int	ft_intlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

void	find_map_width(t_game *game)
{
	int	i;
	int	len;
	int	new_len;

	i = 0;
	len = ft_strlen(game->map.grid[i]);
	while (game->map.grid[i])
	{
		new_len = ft_strlen(game->map.grid[i]);
		if (new_len > len)
			len = new_len;
		i++;
	}
	game->map.width = len;
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	while (haystack[i] != '\0')
	{
		j = 0;
		while ((haystack[i + j] == needle[j]))
		{
			if (needle[j + 1] == '\0')
				return ((char *)&haystack[i]);
			j++;
		}
		i++;
	}
	return ((void *)0);
}

void	free_textures(t_textures textures)
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

void	delete_weapon(t_game *game)
{
	int	i;

	i = 0;
	if (game->weapon.frames_left)
	{
		while (game->weapon.frames_left[i])
		{
			mlx_delete_texture(game->weapon.frames_left[i]);
			i++;
		}
	}
	i = 0;
	if (game->weapon.frames_right)
	{
		while (game->weapon.frames_right[i])
		{
			mlx_delete_texture(game->weapon.frames_right[i]);
			i++;
		}
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
	exit(exit_status);
}
