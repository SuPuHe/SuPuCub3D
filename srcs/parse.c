/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/10/02 15:05:49 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <string.h>

int	get_info(char *file, t_game *game)
{
	(void)game;
	if (!strstr(file, "SO") || !strstr(file, "NO") || !strstr(file, "WE")
			|| !strstr(file, "EA") || !strstr(file, "F") || !strstr(file, "C"))
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
	get_info(tmp, game);
	if (tmp)
	{
		//printf("%s\n", tmp);
		free(tmp);
	}
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
