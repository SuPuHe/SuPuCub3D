/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 12:02:20 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <string.h>

//replace strstr to custom function

void	get_color(char *line, int helper)
{
	char	*colors;
	int		i;

	if (helper)
	{
		colors = ft_strtrim(line, "F ");
		ft_game()->textures.floor_color[0] = ft_atoi(colors);
		i = ft_intlen(ft_game()->textures.floor_color[0]) + 1;
		ft_game()->textures.floor_color[1] = ft_atoi(&colors[i]);
		i += ft_intlen(ft_game()->textures.floor_color[1]) + 1;
		ft_game()->textures.floor_color[2] = ft_atoi(&colors[i]);
	}
	else
	{
		colors = ft_strtrim(line, "C ");
		ft_game()->textures.ceil_color[0] = ft_atoi(colors);
		i = ft_intlen(ft_game()->textures.ceil_color[0]) + 1;
		ft_game()->textures.ceil_color[1] = ft_atoi(&colors[i]);
		i += ft_intlen(ft_game()->textures.ceil_color[1]) + 1;
		ft_game()->textures.ceil_color[2] = ft_atoi(&colors[i]);
	}
}

int	get_info(char **file)
{
	int	i;

	i = 0;
	while (file[i])
	{
		if (strstr(file[i], "NO"))
			ft_game()->textures.north_path = ft_strtrim(file[i], "NO ");
		if (strstr(file[i], "SO"))
			ft_game()->textures.south_path = ft_strtrim(file[i], "SO ");
		if (strstr(file[i], "WE"))
			ft_game()->textures.west_path = ft_strtrim(file[i], "WE ");
		if (strstr(file[i], "EA"))
			ft_game()->textures.east_path = ft_strtrim(file[i], "EA ");
		if (strstr(file[i], "F"))
			get_color(file[i], 1);
		if (strstr(file[i], "C"))
			get_color(file[i], 0);
		i++;
	}
	// if (!ft_game()->textures.north_path || !ft_game()->textures.south_path || !ft_game()->textures.west_path
	// 		|| !ft_game()->textures.east_path || !ft_game()->textures.floor_value || !ft_game()->textures.ceil_value)
	// 		return (print_error("Insufficient rexture or color data"), 0);
	return (1);
}

int	read_map(char *map)
{
	char	*line;
	char	*joined;
	char	*tmp;

	ft_game()->map.fd = open(map, O_RDONLY);
	if (ft_game()->map.fd == -1)
		return (print_error("Error with map name/path"), 0);
	line = get_next_line(ft_game()->map.fd);
	if (!line)
		return (print_error("Empty map or read error"), 0);
	joined = ft_strdup(line);
	if (!joined)
		return(free(line), print_error("Allocation failed"), 0);
	free(line);
	line = get_next_line(ft_game()->map.fd);
	while(line)
	{
		tmp = ft_strjoin(joined, line);
		free(joined);
		joined = tmp;
		free(line);
		line = get_next_line(ft_game()->map.fd);
	}
	char **file = ft_split(tmp, '\n');
	free(tmp);
	if (!get_info(file))
		return (free_split(file), free_textures_path(ft_game()->textures), 0);
	free_split(file);
	return (1);
}

int	parsing_map(char *argv)
{
	if (ft_strncmp(&argv[ft_strlen(argv) - 4], ".cub", 4) != 0)
		return (print_error("Map not in .cub format"), 0);
	if (!read_map(argv))
		return (print_error("Error with getting map"), 0);
	return (1);
}
