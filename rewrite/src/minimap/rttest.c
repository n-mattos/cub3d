/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rttest.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos- <nmattos-@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 12:58:15 by mschippe      #+#    #+#                 */
/*   Updated: 2025/07/14 15:11:40 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/minimap.h"

#define EPSILON 1.e-10
#define TILE_SIZE 40
#define PIXEL_SIZE 1000000

void	drawline_draw(mlx_image_t *img, t_point a, t_point b, uint32_t color, uint32_t *pixels)
{
	uint32_t	x;
	uint32_t	y;
	t_vect		inc;
	t_vect		pos;

	inc.x = ((double)b.x - (double)a.x) / PIXEL_SIZE / LINESMOOTHNESS;
	inc.y = ((double)b.y - (double)a.y) / PIXEL_SIZE / LINESMOOTHNESS;
	pos.x = ((double)a.x / PIXEL_SIZE) * TILE_SIZE;
	pos.y = ((double)a.y / PIXEL_SIZE) * TILE_SIZE;
	while (fabs((double)b.x / PIXEL_SIZE * TILE_SIZE - pos.x) > 0.001
		|| fabs((double)b.y / PIXEL_SIZE * TILE_SIZE - pos.y) > 0.001
		|| pos.x < 0 || pos.y < 0
		|| pos.x >= img->width || pos.y >= img->height)
	{
		x = (uint32_t)(pos.x + 0.5);
		y = (uint32_t)(pos.y + 0.5);
		if (x < img->width && y < img->height)
			pixels[y * img->width + x] = color;
		pos.x += inc.x;
		pos.y += inc.y;
	}
}

void	drawline(mlx_image_t *img, t_point a, t_point b, uint32_t color)
{
	uint32_t	*pixels;

	pixels = (uint32_t *)(img->pixels);
	if (!pixels)
		return;
	if (a.x == b.x && a.y == b.y)
	{
		pixels[a.y * img->width + a.x] = color;
		return;
	}
	drawline_draw(img, a, b, color, pixels);
}

t_vect	calculate_delta(t_vect raydir)
{
	t_vect	delta;

	if (fabs(raydir.x) < EPSILON)
		delta.x = 0;
	else
		delta.x = fabs(1 / raydir.x);
	if (fabs(raydir.y) < EPSILON)
		delta.y = 0;
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

double calculate_perpendicular_distance(t_playerdata p, t_vect raydir, t_point map, int hit_side, t_point step)
{
	double	perp_wall_dist;

	if (hit_side == 0)
		perp_wall_dist = (map.x - p.x + (1 - step.x) / 2) / raydir.x;
	else
		perp_wall_dist = (map.y - p.y + (1 - step.y) / 2) / raydir.y;
	return (perp_wall_dist);
}

t_vect calculate_intersection(t_playerdata p, t_vect raydir, double perp_wall_dist)
{
	t_vect	intersect;

	intersect.x = p.x + raydir.x * perp_wall_dist;
	intersect.y = p.y + raydir.y * perp_wall_dist;
	return (intersect);
}

void	raycast_dda(t_level *lvl, mlx_image_t *img)
{
	t_playerdata	p;
	t_vect			side;
	t_point 		step;
	t_point			map;
	t_vect			raydir;
	t_vect			delta;
	int				x;
	t_vect			intersect;
	double			perp_wall_dist;
	int				hit_side;

	p = *lvl->player;
	x = 0;
	while (x < (int)img->width)
	{
		raydir = calculate_raydir(img, p, x);
		delta = calculate_delta(raydir);
		map = calculate_map(p);
		side = calculate_side(p, raydir, map, delta, &step);

		hit_side = -1; // -1 = no hit, 0 = hit vertical wall, 1 = hit horizontal wall
		calculate_ray(&map, &side, delta, step, &hit_side, lvl);
		(void)hit_side;	// unused variable

		perp_wall_dist = calculate_perpendicular_distance(p, raydir, map, hit_side, step);
		(void)perp_wall_dist; // unused variable

		intersect = calculate_intersection(p, raydir, perp_wall_dist);

		drawline(img, (t_point){p.x * PIXEL_SIZE, p.y * PIXEL_SIZE}, (t_point){intersect.x * PIXEL_SIZE, intersect.y * PIXEL_SIZE}, 0xFFd6ffcf);

		x += (int)img->width / TOTAL_RAYS;
	}
}
