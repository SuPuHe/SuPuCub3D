/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:34:36 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 15:03:29 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	is_invalid_char(char c)
{
	return (c != '0' && c != '1' && c != '2' && c != '3'
		&& c != '4' && c != '5' && c != '6' && c != '7'
		&& c != '8' && c != '9' && c != 'W' && c != 'E'
		&& c != 'N' && c != 'S' && c != 'D');
}

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
		return (0);
	}
	free_split(game->map.copy_map);
	game->map.copy_map = NULL;
	return (1);
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
