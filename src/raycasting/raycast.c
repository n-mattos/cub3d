/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/18 14:39:31 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Performs the DDA (Digital Differential Analyzer) algorithm for raycasting.
 * @param lvl Pointer to the game level structure containing
 * 			  map, textures, and player data.
 * @param mmap Pointer to the minimap image where rays will be drawn.
 * @param frame Pointer to the frame image where walls will be drawn.
 */
void	raycast_dda(t_data *d)
{
	t_raycast		ray;
	t_playerdata	p;
	int				x;

	p = *d->level->player;
	x = 0;
	while (x < (int)d->last_frame->width)
	{
		ray.raydir = calculate_raydir(d->last_frame, p, x);
		ray.delta = calculate_delta(ray.raydir);
		ray.map = calculate_map(p);
		ray.side = calculate_side(p, &ray, ray.map);
		ray.hit_side = NO_HIT;
		calculate_ray((&ray.map), &ray, d->level);
		ray.perp_wall_dist = calculate_perpendicular_distance(p, &ray, ray.map);
		draw_minimap_rays(d->minimap, p, calculate_intersection(
				p, ray.raydir, ray.perp_wall_dist), x);
		draw_textured_wall(&ray, d, x);
		x += (int)d->last_frame->width / TOTAL_RAYS;
	}
}
