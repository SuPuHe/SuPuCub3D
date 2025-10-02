/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/10/02 16:03:19 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <string.h>

//replace strstr to custom function
int	get_info(char **file, t_game *game)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (strstr(file[i], "NO"))
			game->textures.north_path = ft_strtrim(file[i], "NO ");
		if (strstr(file[i], "SO"))
			game->textures.south_path = ft_strtrim(file[i], "SO ");
		if (strstr(file[i], "WE"))
			game->textures.west_path = ft_strtrim(file[i], "WE ");
		if (strstr(file[i], "EA"))
			game->textures.east_path = ft_strtrim(file[i], "EA ");
		if (strstr(file[i], "F"))
			game->textures.floor_value = ft_strtrim(file[i], "F ");
		if (strstr(file[i], "C"))
			game->textures.ceil_value = ft_strtrim(file[i], "C ");
		i++;
	}
	if (!game->textures.north_path || !game->textures.south_path || !game->textures.west_path
			|| !game->textures.east_path || !game->textures.floor_value || !game->textures.ceil_value)
			return (print_error("Insufficient rexture or color data"), 0);
	return (1);
}

int	get_map(char *map, t_game *game)
{
	char	*line;
	char	*joined;
	char	*tmp;

	game->map.fd = open(map, O_RDONLY);
	if (game->map.fd == -1)
		return (print_error("Error with map name/path"), 0);
	line = get_next_line(game->map.fd);
	if (!line)
		return (print_error("Empty map or read error"), 0);
	joined = ft_strdup(line);
	if (!joined)
		return(print_error("Allocation failed"), 0);
	free(line);
	line = get_next_line(game->map.fd);
	while(line)
	{
		tmp = ft_strjoin(joined, line);
		free(joined);
		joined = tmp;
		free(line);
		line = get_next_line(game->map.fd);
	}
	char **file = ft_split(tmp, '\n');
	free(tmp);
	if (!get_info(file, game))
		return (free_split(file), free_textures_path(game->textures), 0);
	free_split(file);
	return (1);
}

int	parsing_map(char *argv, t_game *game)
{
	if (ft_strncmp(&argv[ft_strlen(argv) - 4], ".cub", 4) != 0)
		return (print_error("Map not in .cub format"), 0);
	if (!get_map(argv, game))
		return (print_error("Error with getting map"), 0);
	return (1);
}
