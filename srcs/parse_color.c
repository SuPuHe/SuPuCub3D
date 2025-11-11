/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:48:24 by omizin            #+#    #+#             */
/*   Updated: 2025/11/11 10:25:36 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

uint32_t	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	set_color(void)
{
	t_game	*game;

	game = ft_game();
	game->textures.ceil = get_rgba(game->textures.ceil_color[0],
			game->textures.ceil_color[1], game->textures.ceil_color[2], 255);
	game->textures.floor = get_rgba(game->textures.floor_color[0],
			game->textures.floor_color[1], game->textures.floor_color[2], 255);
}

static int	get_color_part(char *str)
{
	int	value;

	value = ft_atoi(str);
	if (value < 0 || value > 255)
		return (print_error("Number out of rgb range"), 0);
	return (value);
}

static int	set_colors_or_error(int i, char **parts, int helper)
{
	if (i != 3)
	{
		i = 0;
		while (parts[i])
			free(parts[i++]);
		free(parts);
		return (print_error("invalid color format (expected R,G,B)"), 0);
	}
	if (helper)
	{
		ft_game()->textures.floor_color[0] = get_color_part(parts[0]);
		ft_game()->textures.floor_color[1] = get_color_part(parts[1]);
		ft_game()->textures.floor_color[2] = get_color_part(parts[2]);
	}
	else
	{
		ft_game()->textures.ceil_color[0] = get_color_part(parts[0]);
		ft_game()->textures.ceil_color[1] = get_color_part(parts[1]);
		ft_game()->textures.ceil_color[2] = get_color_part(parts[2]);
	}
	set_color();
	return (1);
}

int	get_color(char *line, int helper)
{
	char	*trimmed;
	char	**parts;
	int		i;

	if (helper && ft_game()->textures.floor_color[0] != -1)
		return (print_error("Duplicate floor color definition"), 0);
	if (!helper && ft_game()->textures.ceil_color[0] != -1)
		return (print_error("Duplicate ceiling color definition"), 0);
	if (helper)
		trimmed = ft_strtrim(line, "F ");
	else
		trimmed = ft_strtrim(line, "C ");
	if (!trimmed || trimmed[0] == '\0')
		return (free(trimmed), print_error("Color not provided"), 0);
	parts = ft_split(trimmed, ',');
	free(trimmed);
	i = 0;
	while (parts[i])
		i++;
	if (!set_colors_or_error(i, parts, helper))
		return (0);
	i = 0;
	while (parts[i])
		free(parts[i++]);
	free(parts);
	return (1);
}
