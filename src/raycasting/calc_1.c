/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:42:20 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 16:36:35 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	get_tile_type(t_level *lvl, int tile);

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
t_vect	calculate_side(t_playerdata p, t_raycast *ray, t_point map)
{
	t_vect	side;

	if (ray->raydir.x < 0)
	{
		ray->step.x = -1;
		side.x = ((p.x) - map.x) * ray->delta.x;
	}
	else
	{
		ray->step.x = 1;
		side.x = (map.x + 1.0 - (p.x)) * ray->delta.x;
	}
	if (ray->raydir.y < 0)
	{
		ray->step.y = -1;
		side.y = ((p.y) - map.y) * ray->delta.y;
	}
	else
	{
		ray->step.y = 1;
		side.y = (map.y + 1.0 - (p.y)) * ray->delta.y;
	}
	return (side);
}

/**
 * Calculates the ray direction
 * based on the player's position and the pixel x-coordinate.
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
 * Calculates the ray based on the map coordinates, side distances,
 * delta values, step values, and hit side.
 * @param map The current map coordinates.
 * @param ray The raycast data structure to be updated.
 * @param lvl The level data containing the map and player data.
 * @return void
 */
void	calculate_ray(t_point *map, t_raycast *ray, t_level *lvl, bool check_door)
{
	bool	hit;

	hit = false;
	while (!hit)
	{
		if (ray->side.x < ray->side.y)
		{
			ray->side.x += ray->delta.x;
			map->x += ray->step.x;
			ray->hit_side = VERTICAL;
		}
		else
		{
			ray->side.y += ray->delta.y;
			map->y += ray->step.y;
			ray->hit_side = HORIZONTAL;
		}
		hit = !is_player(lvl->map[map->y][map->x])
			&& lvl->map[map->y][map->x] != FLOOR
			&& lvl->map[map->y][map->x] != DOOR_OPEN;
		if (check_door)
			hit = !is_player(lvl->map[map->y][map->x])
				&& lvl->map[map->y][map->x] != FLOOR;
	}
	ray->tile = get_tile_type(lvl, lvl->map[map->y][map->x]);
}

/**
 * Determines the type of tile based on its character representation.
 * @param tile The character representing the tile.
 * @return An integer representing the tile type (WALL, DOOR, PORTAL, or EMPTY).
 */
static int	get_tile_type(t_level *lvl, int tile)
{
	if (tile == 'D')
		return (DOOR);
	if (tile == 'd')
		return (DOOR_OPEN);
	if (find_portal_node(lvl->portals, tile) != NULL)
		return (PORTAL);
	return (EMPTY);
}
