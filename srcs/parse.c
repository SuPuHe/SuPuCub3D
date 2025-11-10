/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:39:39 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 12:59:19 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Reads the entire contents of a file descriptor into a single string.
 *
 * This function repeatedly calls `get_next_line` to
 * read each line from the file, concatenates all lines into a single
 * dynamically allocated string, and returns it. The caller is responsible
 * for freeing the returned string.
 *
 * @param fd The file descriptor to read from.
 * @return A pointer to the concatenated string containing the
 * whole file contents, or NULL if reading fails or the file is empty.
 */
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

/**
 * @brief Parses the split file contents into the game map.
 *
 * This function retrieves game info, extracts the map grid, creates a copy
 * for internal use, and calculates the map width. It also handles errors
 * related to invalid or empty files.
 *
 * @param split_file Array of strings representing the lines of the file.
 * @param game Pointer to the main game structure to store map data.
 * @return 1 if parsing was successful, 0 otherwise.
 */
int	parse_map_file(char **split_file, t_game *game)
{
	if (!split_file)
		return (print_error("Empty or invalid file"), 0);
	if (!get_info(split_file, game))
	{
		free_split(split_file);
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

/**
 * @brief Opens a file, reads its content, and parses it into the game map.
 *
 * This function opens the specified file, reads its
 * content using `read_whole_file`, splits it into lines,
 * and passes it to `parse_map_file` for parsing.
 *
 * @param file The path to the file to read.
 * @return 1 if the file was successfully read and parsed, 0 otherwise.
 */
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

/**
 * @brief Handles the full parsing process for a given .cub file.
 *
 * This function validates the file extension, reads and parses the file,
 * sets up the player position and direction, and checks map validity.
 *
 * @param argv Path to the .cub map file.
 * @return 1 if parsing and setup were successful, 0 otherwise.
 */
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
