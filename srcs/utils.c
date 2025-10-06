/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:16:29 by omizin            #+#    #+#             */
/*   Updated: 2025/10/06 13:45:14 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	print_error(char *msg)
{
	write(2, RED, 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n"RESET, 6);
	ft_game()->exit = 1;
}

void	free_split(char **lines)
{
	int	i;

	i = 0;
	if (!lines)
		return ;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

void	free_textures_path(t_textures textures)
{
	if (textures.north_path)
		free(textures.north_path);
	if (textures.south_path)
		free(textures.south_path);
	if (textures.east_path)
		free(textures.east_path);
	if (textures.west_path)
		free(textures.west_path);
}

t_game	*ft_game(void)
{
	static t_game	game;

	return (&game);
}

int	ft_intlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

void	find_map_width(t_game *game)
{
	int	i;
	int	len;
	int	new_len;

	i = 0;
	len = ft_strlen(game->map.grid[i]);
	while (game->map.grid[i])
	{
		new_len = ft_strlen(game->map.grid[i]);
		if (new_len > len)
			len = new_len;
		i++;
	}
	game->map.width = len;
}
