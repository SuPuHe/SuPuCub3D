/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:16:29 by omizin            #+#    #+#             */
/*   Updated: 2025/11/10 12:23:54 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Prints an error message in red to standard error
 * and sets the game exit flag.
 *
 * This function outputs the provided error message to
 * file descriptor 2 (stderr), prefixes it with a red
 * color escape sequence, and appends a reset color sequence.
 * It also sets the `exit` flag in the global game structure
 * to indicate termination.
 *
 * @param msg The error message string to print.
 */
void	print_error(char *msg)
{
	write(2, RED, 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n"RESET, 6);
	ft_game()->exit = 1;
}

/**
 * @brief Returns a pointer to the global game instance.
 *
 * This function implements a singleton pattern for the `t_game` structure.
 * It provides access to a single, statically allocated game instance.
 *
 * @return Pointer to the global `t_game` structure.
 */
t_game	*ft_game(void)
{
	static t_game	game;

	return (&game);
}

/**
 * @brief Calculates the length of an integer in decimal representation.
 *
 * This function returns the number of characters required to represent
 * an integer, including the sign for negative numbers.
 *
 * @param n The integer value.
 * @return The number of digits (including sign if negative).
 */
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

/**
 * @brief Finds the maximum width of the map grid and stores it
 *  in the game structure.
 *
 * Iterates through all map rows in `game->map.grid` and determines the longest
 * string length among them. The result is stored in `game->map.width`.
 *
 * @param game Pointer to the main game structure.
 */
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

/**
 * @brief Locates the first occurrence of a substring in a string.
 *
 * Searches for the first occurrence of the substring
 * `needle` in the string `haystack`. Returns a pointer to the
 * beginning of the located substring, or `NULL` if not found.
 *
 * @param haystack The string to be searched.
 * @param needle The substring to search for.
 * @return Pointer to the first occurrence of `needle` in
 * `haystack`, or `NULL` if not found.
 */
char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	while (haystack[i] != '\0')
	{
		j = 0;
		while ((haystack[i + j] == needle[j]))
		{
			if (needle[j + 1] == '\0')
				return ((char *)&haystack[i]);
			j++;
		}
		i++;
	}
	return ((void *)0);
}
