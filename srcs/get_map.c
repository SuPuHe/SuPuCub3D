/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:26:45 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:09:58 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Checks if a line from the file represents a map row.
 *
 * A line is considered a map line if it contains only spaces
 * or digits from '0' to '9'.
 *
 * @param line The line to check.
 * @return int Returns 1 if it is a map line, 0 otherwise.
 */
int	is_map_line(char *line)
{
	int	i;

	i = 0;
	if (!line || !line[0])
		return (0);
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '0' && line[i] != '1'
			&& line[i] != '2' && line[i] != '3' && line[i] != '4'
			&& line[i] != '5' && line[i] != '6' && line[i] != '7'
			&& line[i] != '8' && line[i] != '9')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Finds the starting index of the map in a file.
 *
 * Iterates over the lines in the file to locate the first line
 * that is a valid map line.
 *
 * @param file The array of lines from the file.
 * @return int Returns the index of the first map line, or -1 if none found.
 */
int	get_map_start(char **file)
{
	int	i;
	int	start;

	start = -1;
	i = 0;
	while (file[i])
	{
		if (is_map_line(file[i]))
		{
			start = i;
			break ;
		}
		i++;
	}
	return (start);
}

/**
 * @brief Extracts the map portion from a file.
 *
 * This function locates the map start in the file, counts the number
 * of lines in the map, duplicates them into a new array, and sets
 * the map height in the game structure.
 *
 * @param file The array of lines from the file.
 * @return char** Returns a newly allocated
 * array of map lines, or NULL on failure.
 */
char	**extract_map(char **file)
{
	int		i;
	int		start;
	int		count;
	char	**map;

	start = get_map_start(file);
	if (start == -1)
		return (NULL);
	count = 0;
	while (file[start + count])
		count++;
	map = malloc(sizeof(char *) * (count + 1));
	ft_game()->map.height = count;
	if (!map)
		return (NULL);
	i = 0;
	while (i < count)
	{
		map[i] = ft_strdup(file[start + i]);
		i++;
	}
	map[i] = NULL;
	return (map);
}
