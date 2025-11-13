/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_rays.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omizin <omizin@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:34:28 by omizin            #+#    #+#             */
/*   Updated: 2025/11/13 11:44:00 by omizin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

/**
 * @brief Initializes the minimap raycasting parameters.
 *
 * @param mray Pointer to the minimap ray struct.
 * @param game Pointer to the main game state.
 */
static void	init_mray(t_mray *mray, t_game *game)
{
	mray->num_rays = 150;
	mray->step_sample = 0.05;
	mray->max_dist = 20.0;
	mray->center = game->minimap.minimap_size / 2;
	mray->scale = TILE_SIZE * MINIMAP_SCALE;
	mray->radius_px = game->minimap.minimap_size / 2 - 2;
	mray->i = 0;
}

/**
 * @brief Computes the distance the ray can travel
 * and clamps it to the minimap circle.
 *
 * @param game Pointer to the main game state.
 * @param rc Pointer to the raycast struct for this ray.
 * @param mray Pointer to the minimap ray struct.
 */
static void	clamp_ray_distance(t_game *game, t_raycast *rc, t_mray *mray)
{
	mray->camera_x = (double)mray->i / (double)(mray->num_rays - 1) * 2.0 - 1.0;
	mray->screen_x = (int)round((mray->camera_x + 1.0)
			* (SCREEN_WIDTH - 1) / 2.0);
	if (mray->screen_x < 0)
		mray->screen_x = 0;
	if (mray->screen_x >= SCREEN_WIDTH)
		mray->screen_x = SCREEN_WIDTH - 1;
	init_ray(game, rc, mray->screen_x);
	calculate_step_and_side_dist(game, rc);
	perform_dda(game, rc);
	calculate_wall_distance(game, rc);
	mray->t_hit = rc->perp_wall_dist;
	if (mray->t_hit < 0.0 || !isfinite(mray->t_hit) || !rc->hit)
		mray->t_hit = mray->max_dist;
	mray->dir_len = sqrt(rc->ray_dir_x * rc->ray_dir_x
			+ rc->ray_dir_y * rc->ray_dir_y);
	if (mray->dir_len <= 0.0)
		mray->dir_len = 1e-30;
	mray->t_circle = (double)mray->radius_px / (mray->scale * mray->dir_len);
	if (mray->t_hit > mray->t_circle)
		mray->t_hit = mray->t_circle;
	if (mray->t_hit > mray->max_dist)
		mray->t_hit = mray->max_dist;
}

/**
 * @brief Draws a single ray from the player on the minimap.
 *
 * @param game Pointer to the main game state.
 * @param rc Pointer to the raycast struct for this ray.
 * @param mray Pointer to the minimap ray struct.
 */
static void	draw_single_ray(t_game *game, t_raycast *rc, t_mray *mray)
{
	mray->t = 0.0;
	while (mray->t <= mray->t_hit)
	{
		mray->wx = game->player.x + rc->ray_dir_x * mray->t;
		mray->wy = game->player.y + rc->ray_dir_y * mray->t;
		mray->px = mray->center + (int)((mray->wx
					- game->player.x) * mray->scale);
		mray->py = mray->center + (int)((mray->wy
					- game->player.y) * mray->scale);
		mray->dx = mray->px - mray->center;
		mray->dy = mray->py - mray->center;
		if (mray->dx * mray->dx + mray->dy * mray->dy
			> mray->radius_px * mray->radius_px)
			break ;
		if (mray->px >= 0 && mray->px < game->minimap.minimap_size
			&& mray->py >= 0 && mray->py < game->minimap.minimap_size)
			mlx_put_pixel(game->minimap.img, mray->px, mray->py, 0xFF00FFFF);
		mray->t += mray->step_sample;
	}
}

/**
 * @brief Draws all rays from the player on the minimap.
 *
 * @param game Pointer to the main game state.
 */
void	draw_rays_on_minimap(t_game *game)
{
	t_mray		mray;
	t_raycast	rc;

	init_mray(&mray, game);
	while (mray.i < mray.num_rays)
	{
		clamp_ray_distance(game, &rc, &mray);
		draw_single_ray(game, &rc, &mray);
		mray.i++;
	}
}
