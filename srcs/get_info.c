/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:06:54 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 14:37:49 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	get_player_pos(t_game *game)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	found = 0;
	while (ft_game()->map.grid[i])
	{
		j = 0;
		while (ft_game()->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S'
				|| game->map.grid[i][j] == 'E' || game->map.grid[i][j] == 'W')
			{
				ft_game()->player.x = (double)j + 0.5;
				ft_game()->player.y = (double)i + 0.5;
				found++;
			}
			j++;
		}
		i++;
	}
	if (found != 1)
		return (0);
	return (1);
}

static void	get_info_walls(t_game *game, char **file, int i)
{
	if (ft_strstr(file[i], "NO "))
		game->textures.north_path = ft_strtrim(file[i], "NO ");
	if (ft_strstr(file[i], "SO "))
		game->textures.south_path = ft_strtrim(file[i], "SO ");
	if (ft_strstr(file[i], "WE "))
		game->textures.west_path = ft_strtrim(file[i], "WE ");
	if (ft_strstr(file[i], "EA "))
		game->textures.east_path = ft_strtrim(file[i], "EA ");
}

int	get_info(char **file, t_game *game)
{
	int	i;

	i = 0;
	game->textures.ceil_color[0] = -1;
	game->textures.floor_color[0] = -1;
	while (file[i] && !game->exit)
	{
		get_info_walls(game, file, i);
		if (ft_strstr(file[i], "DOOR "))
			game->textures.door_path = ft_strtrim(file[i], "DOOR ");
		if (ft_strstr(file[i], "F "))
			get_color(file[i], 1);
		if (ft_strstr(file[i], "C "))
			get_color(file[i], 0);
		i++;
	}
	if (!ft_game()->textures.north_path || !ft_game()->textures.south_path
		|| !ft_game()->textures.west_path || !ft_game()->textures.east_path
		|| game->textures.ceil_color[0] == -1
		|| game->textures.floor_color[0] == -1 || ft_game()->exit)
		return (print_error("Insufficient rexture or color data"), 0);
	return (1);
}

int	init_player_dir(t_game *game)
{
	char	dir;

	dir = game->map.grid[(int)game->player.y][(int)game->player.x];
	if (dir == 'E')
	{
		game->player.dir_x = 1.0;
		game->player.plane_y = 0.75;
	}
	else if (dir == 'W')
	{
		game->player.dir_x = -1.0;
		game->player.plane_y = -0.75;
	}
	else if (dir == 'N')
	{
		game->player.dir_y = -1.0;
		game->player.plane_x = 0.75;
	}
	else if (dir == 'S')
	{
		game->player.dir_y = 1.0;
		game->player.plane_x = -0.75;
	}
	return (1);
}
