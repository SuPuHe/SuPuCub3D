/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:26:45 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 16:00:01 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	if (!line || !line[0])
		return (0);
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '0' && line[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

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
