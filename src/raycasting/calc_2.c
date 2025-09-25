/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:42:58 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 16:40:24 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Calculates the perpendicular distance from the player to the wall.
 * @param p Player data containing position and direction.
 * @param raydir Ray direction vector.
 * @param map Map coordinates of the wall hit.
 * @param hit_side Side of the wall hit (0 for vertical, 1 for horizontal wall).
 * @param step Step vector indicating the direction of movement.
 * @return Perpendicular distance to the wall.
 */
double	calculate_perpendicular_distance(
			t_playerdata p, t_raycast *ray, t_point map)
{
	if (ray->hit_side == VERTICAL)
		ray->perp_wall_dist
			= (map.x - p.x + (1 - ray->step.x) / 2) / ray->raydir.x;
	else
		ray->perp_wall_dist
			= (map.y - p.y + (1 - ray->step.y) / 2) / ray->raydir.y;
	return (ray->perp_wall_dist);
}

/**
 * Calculates the intersection point of the ray with the wall.
 * @param p Player data containing position and direction.
 * @param raydir Ray direction vector.
 * @param perp_wall_dist Perpendicular distance to the wall.
 * @return Intersection point as a vector.
 */
t_vect	calculate_intersection(
			t_playerdata p, t_vect raydir, double perp_wall_dist)
{
	t_vect	intersect;

	intersect.x = p.x + raydir.x * perp_wall_dist;
	intersect.y = p.y + raydir.y * perp_wall_dist;
	return (intersect);
}

/**
 * Calculates the exact position on the wall where the ray hits.
 * @param p Pointer to player data containing position and direction.
 * @param hit_side Side of the wall hit (0 for vertical, 1 for horizontal wall).
 * @param perp_wall_dist Perpendicular distance to the wall.
 * @param raydir Ray direction vector.
 * @return The exact x-coordinate on the wall
 * 	where the ray hits (fractional part).
 */
double	calculate_wallx(
			t_playerdata *p, int hit_side, double perp_wall_dist, t_vect raydir)
{
	double	wall_x;

	if (hit_side == VERTICAL)
		wall_x = p->y + perp_wall_dist * raydir.y;
	else
		wall_x = p->x + perp_wall_dist * raydir.x;
	return (wall_x -= (int)wall_x);
}

/**
 * Calculates the delta values for the ray direction.
 * @param raydir The ray direction vector.
 * @return A vector describing the delta value.
 */
t_vect	calculate_delta(t_vect raydir)
{
	t_vect	delta;

	if (fabs(raydir.x) < EPSILON)
		delta.x = 1e30;
	else
		delta.x = fabs(1 / raydir.x);
	if (fabs(raydir.y) < EPSILON)
		delta.y = 1e30;
	else
		delta.y = fabs(1 / raydir.y);
	return (delta);
}
