/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:34:36 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 13:00:07 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Checks if a character is invalid in the map.
 *
 * Valid map characters include '0'-'9' for tiles
 * and 'W', 'E', 'N', 'S' for player start.
 *
 * @param c Character to check.
 * @return Non-zero if the character is invalid, 0 otherwise.
 */
static int	is_invalid_char(char c)
{
	return (c != '0' && c != '1' && c != '2' && c != '3'
		&& c != '4' && c != '5' && c != '5' && c != '6'
		&& c != '7' && c != '8' && c != '9' && c != 'W'
		&& c != 'E' && c != 'N' && c != 'S');
}

/**
 * @brief Performs a flood-fill algorithm to check map validity.
 *
 * This function recursively marks reachable positions from (x, y) and ensures
 * the map is closed and contains only valid characters.
 * It stops at walls and doors.
 *
 * @param game Pointer to the game structure containing the map.
 * @param x X-coordinate to start the flood-fill.
 * @param y Y-coordinate to start the flood-fill.
 * @return 1 if the map is valid from this position,
 * 0 if an invalid tile or open space is detected.
 */
int	flood_fill(t_game *game, int x, int y)
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
	if (ft_strchr(MAP_WALLS_DOORS, c))
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

/**
 * @brief Validates the map using flood-fill from the player's position.
 *
 * This function ensures the map is fully enclosed and contains only
 * valid characters. It frees the temporary copy map after checking.
 *
 * @return 1 if the map is valid, 0 otherwise.
 */
int	check_map_valid(void)
{
	t_game	*game;

	game = ft_game();
	if (!flood_fill(game, game->player.x, game->player.y))
	{
		print_error("Map is not closed or contains invalid chars");
		return (0);
	}
	free_split(game->map.copy_map);
	game->map.copy_map = NULL;
	return (1);
}

/**
 * @brief Splits the content of a file into lines.
 *
 * This function uses `ft_split` to split a string by newline characters and
 * frees the original content string.
 *
 * @param content String containing the full file content.
 * @return Null-terminated array of strings (lines), or NULL if content is NULL.
 */
char	**split_file_lines(char *content)
{
	char	**split_file;

	if (!content)
		return (NULL);
	split_file = ft_split(content, '\n');
	free(content);
	return (split_file);
}
