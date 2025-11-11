/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpushkar <vpushkar@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:48:24 by omizin            #+#    #+#             */
/*   Updated: 2025/11/11 10:43:36 by vpushkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Combines individual RGBA components into a single 32-bit color value.
 *
 * The resulting format is 0xRRGGBBAA.
 *
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @param a Alpha component (0-255)
 * @return 32-bit color value in RGBA format.
 */
uint32_t	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

/**
 * @brief Sets the ceiling and floor colors in the game textures.
 *
 * Uses the RGB values stored in `textures.ceil_color` and `textures.floor_color`
 * and converts them into 32-bit RGBA values with full opacity (alpha = 255).
 */
void	set_color(void)
{
	t_game	*game;

	game = ft_game();
	game->textures.ceil = get_rgba(game->textures.ceil_color[0],
			game->textures.ceil_color[1], game->textures.ceil_color[2], 255);
	game->textures.floor = get_rgba(game->textures.floor_color[0],
			game->textures.floor_color[1], game->textures.floor_color[2], 255);
}

/**
 * @brief Parses a string to get a single RGB component.
 *
 * Ensures the value is within the valid range (0-255).
 *
 * @param str String containing the number to parse.
 * @return Integer value of the color component (0-255), or 0 if out of range.
 */
static int	get_color_part(char *str)
{
	int	value;

	value = ft_atoi(str);
	if (value < 0 || value > 255)
		return (print_error("Number out of rgb range"), 0);
	return (value);
}

/**
 * @brief Sets ceiling or floor colors from string parts or returns an error.
 *
 * Validates the number of parts and assigns RGB values to the appropriate
 * texture (ceiling or floor).
 *
 * @param i Number of color parts in the array.
 * @param parts Array of strings representing R, G, B values.
 * @param helper Non-zero if setting floor color, zero if ceiling color.
 * @return 1 on success, 0 on error.
 */
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

/**
 * @brief Parses a line from the map file to extract ceiling or floor color.
 *
 * Checks for duplicate color definitions, trims the line, splits by commas,
 * validates RGB components (0-255), and stores them in the texture array.
 * Frees allocated memory before returning.
 *
 * @param line Line from the map file containing color information.
 * @param helper Non-zero to set floor color, zero to set ceiling color.
 * @return 1 if color parsing and setting succeeded, 0 on error.
 */
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
	return (free(parts), 1);
}
