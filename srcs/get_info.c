/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:06:54 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:09:42 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Finds the player's starting position in the map.
 *
 * Iterates through the map grid to locate a cell containing
 * a player start direction ('N', 'S', 'E', 'W'). Sets the
 * player's x and y coordinates to the center of that cell.
 *
 * @param game Pointer to the game structure.
 * @return int Returns 1 if exactly one player position is found, 0 otherwise.
 */
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

/**
 * @brief Loads wall texture paths from a file line.
 *
 * Checks for the presence of "NO", "SO", "WE", or "EA"
 * in the specified file line and trims the line to extract
 * the texture path, storing it in the game textures structure.
 *
 * @param game Pointer to the game structure.
 * @param file The array of file lines.
 * @param i The index of the current line in the file.
 */
static void	get_info_walls(t_game *game, char **file, int i)
{
	if (ft_strstr(file[i], "NO"))
		game->textures.north_path = ft_strtrim(file[i], "NO ");
	if (ft_strstr(file[i], "SO"))
		game->textures.south_path = ft_strtrim(file[i], "SO ");
	if (ft_strstr(file[i], "WE"))
		game->textures.west_path = ft_strtrim(file[i], "WE ");
	if (ft_strstr(file[i], "EA"))
		game->textures.east_path = ft_strtrim(file[i], "EA ");
}

/**
 * @brief Parses all relevant texture and color information from a file.
 *
 * Iterates over the file lines to find wall textures ("NO", "SO", "WE", "EA"),
 * door texture ("DOOR"), floor color ("F "), and ceiling color ("C ").
 *
 * @param file The array of file lines.
 * @param game Pointer to the game structure.
 * @return int Returns 1 if all necessary textures are loaded, 0 on error.
 */
int	get_info(char **file, t_game *game)
{
	int	i;

	i = 0;
	while (file[i] && !game->exit)
	{
		get_info_walls(game, file, i);
		if (ft_strstr(file[i], "DOOR"))
			game->textures.door_path = ft_strtrim(file[i], "DOOR ");
		if (ft_strstr(file[i], "F "))
			get_color(file[i], 1);
		if (ft_strstr(file[i], "C "))
			get_color(file[i], 0);
		i++;
	}
	if (!ft_game()->textures.north_path || !ft_game()->textures.south_path
		|| !ft_game()->textures.west_path || !ft_game()->textures.east_path
		|| ft_game()->exit)
		return (print_error("Insufficient rexture or color data"), 0);
	return (1);
}

/**
 * @brief Initializes the player's direction vector and camera plane.
 *
 * Sets the player's dir_x/dir_y and plane_x/plane_y values based
 * on the starting map cell ('N', 'S', 'E', 'W') to determine the
 * initial viewing direction and field of view.
 *
 * @param game Pointer to the game structure.
 * @return int Returns 1 on success.
 */
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
