/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rttest.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/29 12:06:30 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/minimap.h"

#define EPSILON 1.e-10

void	drawline_draw(mlx_image_t *img, t_point a, t_point b, uint32_t color, uint32_t *pixels)
{
	uint32_t	x;
	uint32_t	y;
	t_vect		inc;
	t_vect		pos;

	inc.x = ((double)b.x - (double)a.x) / LINESMOOTHNESS;
	inc.y = ((double)b.y - (double)a.y) / LINESMOOTHNESS;
	pos.x = ((double)a.x);
	pos.y = ((double)a.y);
	while (fabs((double)b.x - pos.x) > 0.001
		|| fabs((double)b.y - pos.y) > 0.001)
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

void	set_start_end(int *y_start, int *y_end, t_point a, t_point b, mlx_image_t *img)
{
		if (a.y < b.y)
		*y_start = a.y;
		else
			*y_start = b.y;
		if (a.y > b.y)
			*y_end = a.y;
		else
			*y_end = b.y;
		if (*y_start < 0)
			*y_start = 0;
		if (*y_end >= (int)img->height)
			*y_end = img->height - 1;
}

void	drawvert(mlx_image_t *img, t_point a, t_point b, uint32_t color)
{
	uint32_t	*pixels;
	int			y_start;
	int			y_end;
	int			y;

	pixels = (uint32_t *)img->pixels;
	if (a.x == b.x)
	{
		int x = a.x;
		if (x < 0 || x >= (int)img->width)
			return;
		set_start_end(&y_start, &y_end, a, b, img);
		y = y_start;
		while (y <= y_end)
		{
			pixels[y * img->width + x] = color;
			y++;
		}
	}
}


void	draw_wall(mlx_image_t *img, double perp_dist, int side, int x)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	uint32_t	color;
	
	line_height = (int)(IMG_HEIGHT / perp_dist);
	draw_start = -line_height / 2 + IMG_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + IMG_HEIGHT / 2;
	if (draw_end >= IMG_HEIGHT)
		draw_end = IMG_HEIGHT - 1;
	if (side == 0) // Colors temporary, we will replace these with textures
		color = 0xFF00FFFF;
	else
		color = 0xFFFF00FF;
	drawvert(img, (t_point){x, draw_start}, (t_point){x, draw_end}, color);
}

void	raycast_dda(t_level *lvl, mlx_image_t *mmap, mlx_image_t *frame)
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
	while (x < (int)mmap->width)
	{
		raydir = calculate_raydir(mmap, p, x);
		delta = calculate_delta(raydir);
		map = calculate_map(p);
		side = calculate_side(p, raydir, map, delta, &step);

		hit_side = -1; // -1 = no hit, 0 = hit vertical wall, 1 = hit horizontal wall
		calculate_ray(&map, &side, delta, step, &hit_side, lvl);

		perp_wall_dist = calculate_perpendicular_distance(p, raydir, map, hit_side, step);
		intersect = calculate_intersection(p, raydir, perp_wall_dist);
		if (x >= 0 && x <= (int)mmap->width - 1 && x % (100 / PERCENTAGE_RAYS) == 0)
			drawline(mmap, (t_point){p.x * RECT_SIZE, p.y * RECT_SIZE}, (t_point){intersect.x * RECT_SIZE, intersect.y * RECT_SIZE}, 0xFFd6ffcf);
		// // all this texturing stuff below should probably be its own function but currently unsure what makes snese
		// int		txt_id;
		// double	wall_x;
		// int		txt_x;
		// double	step;
		// int		line_height;
		
		// txt_id = lvl->map[map.x][map.y] - 1;
		// if (hit_side == 0)
		// 	wall_x = lvl->player->y + perp_wall_dist * raydir.y;
		// else
		// 	wall_x = lvl->player->x + perp_wall_dist * raydir.x;
		// wall_x -= floor(wall_x);
		// txt_x = (int)(wall_x * 64.0); // txt width currently hardcoded to 64, not sure if/how/why we would support variable sizes
		// if ((hit_side == 0 && raydir.x > 0) || (hit_side == 1 && raydir.y < 1))
		// 	txt_x = 64 - txt_x - 1; // NOTE: also hardcoded width
		// line_height = (int)(IMG_HEIGHT / perp_wall_dist);
		// step = 1.0 * 64 / line_height; // NOTE: also hardcoded width
		// man all of this needs stuff from the draw wall function or drawvert function and really i should just combine them all somehow rip


		//laskjfdjldsf yep
		draw_wall(frame, perp_wall_dist, hit_side, x);
		x += (int)mmap->width / TOTAL_RAYS;
	}
}
