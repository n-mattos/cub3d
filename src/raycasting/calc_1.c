/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:42:20 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/18 13:39:56 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

/**
 * Calculates the side distances based on the player's position,
 * ray direction, and the map coordinates.
 * @param p The player data containing position and direction.
 * @param raydir The ray direction vector.
 * @param map The current map coordinates.
 * @param delta The delta values for the ray direction.
 * @param step Pointer to a point structure to store the step values.
 * @return A vector describing the side distance.
 */
t_vect	calculate_side(t_playerdata p, t_vect raydir, t_point map, t_vect delta, t_point *step)
{
	t_vect	side;

	if (raydir.x < 0)
	{
		step->x = -1;
		side.x = ((p.x) - map.x) * delta.x;
	}
	else
	{
		step->x = 1;
		side.x = (map.x + 1.0 - (p.x)) * delta.x;
	}
	if (raydir.y < 0)
	{
		step->y = -1;
		side.y = ((p.y) - map.y) * delta.y;
	}
	else
	{
		step->y = 1;
		side.y = (map.y + 1.0 - (p.y)) * delta.y;
	}
	return side;
}

/**
 * Calculates the ray direction based on the player's position and the pixel x-coordinate.
 * @param img The image structure containing the width.
 * @param p The player data containing position and direction.
 * @param x The pixel x-coordinate.
 * @return A vector describing the ray direction.
 */
t_vect	calculate_raydir(mlx_image_t *img, t_playerdata p, int x)
{
	double	camera_x;
	t_vect	raydir;

	camera_x = 2.0 * x / (double)img->width - 1.0;

	raydir.x = p.dir_x + p.plane_x * camera_x;
	raydir.y = p.dir_y + p.plane_y * camera_x;
	return (raydir);
}

/**
 * Calculates the map coordinates based on the player's position.
 * @param p The player data containing position.
 * @return A point structure containing the map coordinates.
 */
t_point	calculate_map(t_playerdata p)
{
	t_point	map;

	map.x = (int)p.x;
	map.y = (int)p.y;
	return (map);
}

/**
 * Calculates the ray based on the map coordinates, side distances, delta values,
 * step values, and hit side.
 * @param map The current map coordinates.
 * @param side The side distance vector.
 * @param delta The delta values for the ray direction.
 * @param step The step values for the ray direction.
 * @param hit_side Pointer to an integer to store which side was hit (0 for x, 1 for y).
 * @param lvl The level data containing the map and player data.
 * @return void
 */
void	calculate_ray(t_point *map, t_vect *side, t_vect delta, t_point step, int *hit_side, t_level *lvl)
{
	bool	hit;

	hit = false;
	while (!hit)
	{
		if (side->x < side->y)
		{
			side->x += delta.x;
			map->x += step.x;
			hit = lvl->map[map->y][map->x] == '1';
			*hit_side = VERTICAL;
		}
		else
		{
			side->y += delta.y;
			map->y += step.y;
			hit = lvl->map[map->y][map->x] == '1';
			*hit_side = HORIZONTAL;
		}
	}
}
