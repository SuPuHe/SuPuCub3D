/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:01:05 by omizin            #+#    #+#             */
/*   Updated: 2025/10/02 16:03:32 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
		return (print_error("Usage: ./cub maps/map.cub"), 1);
	game = ft_calloc(1, sizeof(t_game));
	if (!parsing_map(argv[1], game))
		return(free(game), 1);
	//printf("%s\n%s\n%s\n%s\n%s\n%s\n", game->textures.ceil_value, game->textures.floor_value, game->textures.south_path, game->textures.north_path, game->textures.west_path, game->textures.east_path);
	free_textures_path(game->textures);
	free(game);
	return (0);
}
