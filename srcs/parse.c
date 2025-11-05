/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/11/05 11:38:17 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <string.h>

//replace strstr to custom function

static int	is_invalid_char(char c)
{
	return (c != '0' && c != '1' && c != '2' && c != '3'
		&& c != '4' && c != '5' && c != '5' && c != '6'
		&& c != '7' && c != '8' && c != '9' && c != 'W'
		&& c != 'E' && c != 'N' && c != 'S');
}

static int	flood_fill(t_game *game, int x, int y)
{
	int		rows;
	int		cols;
	char	c;

	rows = game->map.height;
	if (y < 0 || y >= rows)
		return (0);
	cols = ft_strlen(game->map.copy_map[y]);
	if (x < 0 || x >= cols)
		return (0);
	c = game->map.copy_map[y][x];
	if (ft_strchr(MAP_WALLS, c))
		return (1);
	if (is_invalid_char(c))
		return (0);
	game->map.copy_map[y][x] = 'F';
	if (!flood_fill(game, x + 1, y))
		return (0);
	if (!flood_fill(game, x - 1, y))
		return (0);
	if (!flood_fill(game, x, y + 1))
		return (0);
	if (!flood_fill(game, x, y - 1))
		return (0);
	return (1);
}

int	check_map_valid(void)
{
	t_game	*game;

	game = ft_game();
	if (!flood_fill(game, game->player.x, game->player.y))
	{
		print_error("Map is not closed or contains invalid chars");
		free_split(game->map.copy_map);
		return (0);
	}
	free_split(game->map.copy_map);
	return (1);
}

char	*read_whole_file(int fd)
{
	char	*line;
	char	*joined;
	char	*tmp;

	line = get_next_line(fd);
	if (!line)
		return (NULL);
	joined = ft_strdup(line);
	free(line);
	if (!joined)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		tmp = ft_strjoin(joined, line);
		free(joined);
		free(line);
		line = get_next_line(fd);
		if (!tmp)
			return (NULL);
		joined = tmp;
	}
	return (joined);
}

char	**split_file_lines(char *content)
{
	char	**split_file;

	if (!content)
		return (NULL);
	split_file = ft_split(content, '\n');
	free(content);
	return (split_file);
}

int	parse_map_file(char **split_file, t_game *game)
{
	if (!split_file)
		return (print_error("Empty or invalid file"), 0);
	if (!get_info(split_file, game))
	{
		free_split(split_file);
		free_textures_path(game->textures);
		return (0);
	}
	game->map.grid = extract_map(split_file);
	game->map.copy_map = extract_map(split_file);
	free_split(split_file);
	if (!game->map.grid)
		return (print_error("Failed to extract map"), 0);
	find_map_width(game);
	return (1);
}

int	read_file(char *file)
{
	t_game	*game;
	char	*content;
	char	**split_file;

	game = ft_game();
	game->map.fd = open(file, O_RDONLY);
	if (game->map.fd == -1)
		return (print_error("Error with file name/path"), 0);
	content = read_whole_file(game->map.fd);
	if (!content)
		return (print_error("Empty file or read error"), 0);
	split_file = split_file_lines(content);
	return (parse_map_file(split_file, game));
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
	if (!check_map_valid())
		return (0);
	if (!init_player_dir(game))
		return (0);
	return (1);
}
