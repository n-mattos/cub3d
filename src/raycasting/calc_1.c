/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:42:20 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/11 13:48:03 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

t_vect	calculate_raydir(mlx_image_t *img, t_playerdata p, int x)
{
	double	camera_x;
	t_vect	raydir;

	camera_x = 2.0 * x / (double)img->width - 1.0;

	raydir.x = p.dir_x + p.plane_x * camera_x;
	raydir.y = p.dir_y + p.plane_y * camera_x;
	return (raydir);
}

t_point	calculate_map(t_playerdata p)
{
	t_point	map;

	map.x = (int)p.x;
	map.y = (int)p.y;
	return (map);
}

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
			*hit_side = 0;
		}
		else
		{
			side->y += delta.y;
			map->y += step.y;
			hit = lvl->map[map->y][map->x] == '1';
			*hit_side = 1;
		}
	}
}
