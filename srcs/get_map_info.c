/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_info.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:06:54 by omizin            #+#    #+#             */
/*   Updated: 2025/11/11 11:43:12 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Loads wall texture paths from a file line.
 *
 * Checks for the presence of "WE", "EA"
 * in the specified file line and trims the line to extract
 * the texture path, storing it in the game textures structure.
 * Sets exit flag if duplicate texture is detected.
 *
 * @param game Pointer to the game structure.
 * @param file The array of file lines.
 * @param i The index of the current line in the file.
 */
static void	get_info_walls_west_east(t_game *game, char **file, int i)
{
	if (ft_strstr(file[i], "WE "))
	{
		if (game->textures.west_path)
			game->exit = 1;
		else
			game->textures.west_path = ft_strtrim(file[i], "WE ");
	}
	if (ft_strstr(file[i], "EA "))
	{
		if (game->textures.east_path)
			game->exit = 1;
		else
			game->textures.east_path = ft_strtrim(file[i], "EA ");
	}
}

/**
 * @brief Loads wall texture paths from a file line.
 *
 * Checks for the presence of "NO", "SO", "WE", or "EA"
 * in the specified file line and trims the line to extract
 * the texture path, storing it in the game textures structure.
 * Sets exit flag if duplicate texture is detected.
 *
 * @param game Pointer to the game structure.
 * @param file The array of file lines.
 * @param i The index of the current line in the file.
 */
static void	get_info_walls(t_game *game, char **file, int i)
{
	if (ft_strstr(file[i], "NO "))
	{
		if (game->textures.north_path)
			game->exit = 1;
		else
			game->textures.north_path = ft_strtrim(file[i], "NO ");
	}
	if (ft_strstr(file[i], "SO "))
	{
		if (game->textures.south_path)
			game->exit = 1;
		else
			game->textures.south_path = ft_strtrim(file[i], "SO ");
	}
	get_info_walls_west_east(game, file, i);
}

/**
 * @brief Processes door texture and color information from a file line.
 *
 * Checks for "DOOR", "F " (floor), and "C " (ceiling) in the file line.
 * Sets exit flag if duplicate definitions are found.
 *
 * @param game Pointer to the game structure.
 * @param file The array of file lines.
 * @param i The index of the current line in the file.
 */
static void	process_door_colors(t_game *game, char **file, int i)
{
	if (ft_strstr(file[i], "DOOR "))
	{
		if (game->textures.door_path)
			game->exit = 1;
		else
			game->textures.door_path = ft_strtrim(file[i], "DOOR ");
	}
	if (ft_strstr(file[i], "F ") && !get_color(file[i], 1))
		game->exit = 1;
	if (ft_strstr(file[i], "C ") && !get_color(file[i], 0))
		game->exit = 1;
}

/**
 * @brief Parses all relevant texture and color information from a file.
 *
 * Iterates over the file lines to find wall textures ("NO", "SO", "WE", "EA"),
 * door texture ("DOOR"), floor color ("F "), and ceiling color ("C ").
 * Validates that all required textures and colors are present.
 *
 * @param file The array of file lines.
 * @param game Pointer to the game structure.
 * @return int Returns 1 if all necessary textures are loaded, 0 on error.
 */
int	get_info(char **file, t_game *game)
{
	int	i;

	i = -1;
	game->textures.ceil_color[0] = -1;
	game->textures.floor_color[0] = -1;
	while (file[++i] && !game->exit)
	{
		get_info_walls(game, file, i);
		process_door_colors(game, file, i);
	}
	if (!ft_game()->textures.north_path || !ft_game()->textures.south_path
		|| !ft_game()->textures.west_path || !ft_game()->textures.east_path
		|| game->textures.ceil_color[0] == -1
		|| game->textures.floor_color[0] == -1 || ft_game()->exit)
	{
		if (ft_game()->exit)
			return (print_error("Duplicate texture or color definition"), 0);
		return (print_error("Insufficient texture or color data"), 0);
	}
	return (1);
}
