/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:26:45 by omizin            #+#    #+#             */
/*   Updated: 2025/10/03 13:28:43 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_header_line(char *line)
{
	if (!line || line[0] == '\0')
		return 1;
	if (ft_strncmp(line, "NO", 2) == 0)
		return 1;
	if (ft_strncmp(line, "SO", 2) == 0)
		return 1;
	if (ft_strncmp(line, "WE", 2) == 0)
		return 1;
	if (ft_strncmp(line, "EA", 2) == 0)
		return 1;
	if (line[0] == 'F')
		return 1;
	if (line[0] == 'C')
		return 1;
	return 0;
}

char	**extract_map(char **split_file)
{
	int		start;
	int		size;
	int		i;
	char	**map;

	start = 0;
	size = 0;
	while (split_file[start] && is_header_line(split_file[start]))
		start++;
	i = start;
	while (split_file[i])
	{
		size++;
		i++;
	}
	map = malloc((size + 1) * sizeof(char *));
	if (!map)
		return NULL;
	i = 0;
	while (split_file[start])
	{
		map[i] = ft_strdup(split_file[start]);
		start++;
		i++;
	}
	map[i] = NULL;
	return (map);
}
