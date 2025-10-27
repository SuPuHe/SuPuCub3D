#include "main.h"

void	init_ui()
{
	t_game	*game;

	game = ft_game();
	mlx_image_to_window(game->mlx, game->tx_images.billy, SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 250);
}
