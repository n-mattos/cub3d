/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/10/03 14:22:30 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	calculate_gifs(t_data *d);

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
	t_raycast		door;
	t_playerdata	p;
	int				x;

	p = *d->level->player;
	x = 0;
	calculate_gifs(d);
	while (x < (int)d->last_frame->width)
	{
		ray = single_ray(d, p, x);
		calculate_ray((&ray.map), &ray, d->level, false);
		ray.perp_wall_dist = calculate_perpendicular_distance(p, &ray, ray.map);
		draw_minimap_rays(d, p, calculate_intersection(
				p, ray.raydir, ray.perp_wall_dist), x);
		door = single_ray(d, p, x);
		calculate_ray((&door.map), &door, d->level, true);
		door.perp_wall_dist = calculate_perpendicular_distance(
				p, &door, door.map);
		draw_textured_wall(&ray, d, x, false);
		if (door.tile == DOOR || door.tile == DOOR_OPEN)
			if (door.perp_wall_dist >= 0.2)
				draw_textured_wall(&door, d, x, true);
		x += (int)d->last_frame->width / TOTAL_RAYS;
	}
}

/**
 * Casts a single ray and calculates all necessary parameters for rendering.
 * @param d Pointer to the game data structure.
 * @param p Player data containing position and direction.
 * @param x Current x-coordinate on the screen.
 * @returns t_raycast; Structure containing all calculated ray parameters.
 */
t_raycast	single_ray(t_data *d, t_playerdata p, int x)
{
	t_raycast	ray;

	ray.frame = d->gif_portal->frame;
	ray.raydir = calculate_raydir(d->last_frame, p, x);
	ray.delta = calculate_delta(ray.raydir);
	ray.map = calculate_map(p);
	ray.side = calculate_side(p, &ray, ray.map);
	ray.hit_side = NO_HIT;
	return (ray);
}

static void	calculate_gifs(t_data *d)
{
	if (d->gif_portal->current - d->gif_portal->last > 0.1)
	{
		d->gif_portal->frame = (d->gif_portal->frame + 1) % 6;
		d->gif_portal->last = d->gif_portal->current;
	}
}
