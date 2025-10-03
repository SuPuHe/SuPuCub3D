/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 14:54:56 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <string.h>

//replace strstr to custom function

int	read_file(char *file)
{
	char	*line;
	char	*joined;
	char	*tmp;
	t_game	*game;

	game = ft_game();
	game->map.fd = open(file, O_RDONLY);
	if (game->map.fd == -1)
		return (print_error("Error with file name/path"), 0);
	line = get_next_line(ft_game()->map.fd);
	if (!line)
		return (print_error("Empty file or read error"), 0);
	joined = ft_strdup(line);
	if (!joined)
		return (free(line), print_error("Allocation failed"), 0);
	free(line);
	line = get_next_line(ft_game()->map.fd);
	while (line)
	{
		tmp = ft_strjoin(joined, line);
		free(joined);
		joined = tmp;
		free(line);
		line = get_next_line(ft_game()->map.fd);
	}
	char	**split_file = ft_split(tmp, '\n');
	free(tmp);
	if (!get_info(split_file, game))
		return (free_split(split_file), free_textures_path(ft_game()->textures), 0);
	ft_game()->map.grid = extract_map(split_file);
	free_split(split_file);
	if (!ft_game()->map.grid)
		return (print_error("Failed to extract map"), 0);
	return (1);
}

int	parsing_file(char *argv)
{
	t_game	*game;

	game = ft_game();
	if (ft_strncmp(&argv[ft_strlen(argv) - 4], ".cub", 4) != 0)
		return (print_error("File not in .cub format"), 0);
	if (!read_file(argv))
		return (print_error("Error with getting file"), 0);
	if (!get_player_pos(game))
		return (print_error("Player not found or "
				"multiply player declaration"), 0);
	return (1);
}
