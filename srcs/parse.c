/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 13:31:03 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <string.h>

//replace strstr to custom function

int	get_info(char **file)
{
	int	i;

	i = 0;
	while (file[i] && !ft_game()->exit)
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
	if (!ft_game()->textures.north_path || !ft_game()->textures.south_path || !ft_game()->textures.west_path
			|| !ft_game()->textures.east_path)
			return (print_error("Insufficient rexture or color data"), 0);
	return (1);
}

int	read_file(char *file)
{
	char	*line;
	char	*joined;
	char	*tmp;

	ft_game()->map.fd = open(file, O_RDONLY);
	if (ft_game()->map.fd == -1)
		return (print_error("Error with file name/path"), 0);
	line = get_next_line(ft_game()->map.fd);
	if (!line)
		return (print_error("Empty file or read error"), 0);
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
	char **split_file = ft_split(tmp, '\n');
	free(tmp);
	if (!get_info(split_file))
		return (free_split(split_file), free_textures_path(ft_game()->textures), 0);
	ft_game()->map.grid = extract_map(split_file);
	free_split(split_file);
	if (!ft_game()->map.grid)
		return (print_error("Failed to extract map"), 0);
	return (1);
}

int	parsing_file(char *argv)
{
	if (ft_strncmp(&argv[ft_strlen(argv) - 4], ".cub", 4) != 0)
		return (print_error("File not in .cub format"), 0);
	if (!read_file(argv))
		return (print_error("Error with getting file"), 0);
	return (1);
}
