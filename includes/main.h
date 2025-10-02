/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:05:21 by omizin            #+#    #+#             */
/*   Updated: 2025/10/02 15:38:18 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# define RESET		"\033[0m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"

# include "libft.h"
# include "get_next_line.h"
# include "structs.h"

# include <stdio.h>
# include <fcntl.h>

void	print_error(char *msg);
int		parsing_map(char *argv, t_game *game);
void	free_split(char **lines);
void	free_textures_path(t_textures textures);

#endif
