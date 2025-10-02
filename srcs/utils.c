/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 13:16:29 by omizin            #+#    #+#             */
/*   Updated: 2025/10/02 15:40:55 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	print_error(char *msg)
{
	write(2, RED, 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n"RESET, 6);
}

void	free_split(char **lines)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

void	free_textures_path(t_textures textures)
{
	if (textures.ceil_value)
		free(textures.ceil_value);
	if (textures.floor_value)
		free(textures.floor_value);
	if (textures.north_path)
		free(textures.north_path);
	if (textures.south_path)
		free(textures.south_path);
	if (textures.east_path)
		free(textures.east_path);
	if (textures.west_path)
		free(textures.west_path);
}
