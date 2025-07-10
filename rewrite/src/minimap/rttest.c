/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rttest.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/10 16:04:28 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/minimap.h"

#define EPSILON 1.e-10
#define TILE_SIZE 40
#define PIXEL_SIZE 1000000

void	drawline(mlx_image_t *img, t_point a, t_point b, uint32_t color)
{
	uint32_t	*pixels;
	uint32_t	x;
	uint32_t	y;

	x = 0;
	y = 0;
	pixels = (uint32_t *)(img->pixels);
	if (!pixels)
		return;
	if (a.x == b.x && a.y == b.y)
	{
		pixels[a.y * img->width + a.x] = color;
		return;
	}
	double	dx = (double)((double)b.x - (double)a.x) / PIXEL_SIZE;
	double	dy = (double)((double)b.y - (double)a.y) / PIXEL_SIZE;
	double	x_inc = dx / 1000; // bigger division => smoother line; more iterations
	double	y_inc = dy / 1000;
	double	x_pos = ((double)a.x / PIXEL_SIZE) * TILE_SIZE;
	double	y_pos = ((double)a.y / PIXEL_SIZE) * TILE_SIZE;
	while (fabs((double)b.x / PIXEL_SIZE * TILE_SIZE - x_pos) > 0.001 || fabs((double)b.y / PIXEL_SIZE * TILE_SIZE - y_pos) > 0.001)
	{
		if (x_pos < 0 || y_pos < 0 || x_pos >= img->width || y_pos >= img->height)
			break;
		// if wall is hit, stop drawing
		x = (uint32_t)(x_pos + 0.5);	// + 0.5 to round to the nearest integer
		y = (uint32_t)(y_pos + 0.5);
		if (x < img->width && y < img->height)
			pixels[y * img->width + x] = color;
		x_pos += x_inc;
		y_pos += y_inc;
	}
}

void	drawline_angle(mlx_image_t *img, t_point a, double angle, double euclidean, uint32_t color)
{
	t_point	b;

	b.x = a.x + cos(angle) * PIXEL_SIZE * euclidean;
	b.y = a.y + sin(angle) * PIXEL_SIZE * euclidean;
	drawline(img, a, b, color);
}

void	raycast_dda(t_level *lvl, mlx_image_t *img)
{
	t_playerdata p = *lvl->player;

	int x = 0;
	while (x < (int)img->width)
	{
		double camera_x = 2.0 * x / (double)img->width - 1.0;
		double raydir_x = p.dir_x + p.plane_x * camera_x;
		double raydir_y = p.dir_y + p.plane_y * camera_x;

		double delta_x;
		double delta_y;
		if (fabs(raydir_x) < EPSILON)
			delta_x = 0;
		else
			delta_x = fabs(1 / raydir_x);
		if (fabs(raydir_y) < EPSILON)
			delta_y = 0;
		else
			delta_y = fabs(1 / raydir_y);

		int map_x = p.x;
		int map_y = p.y;
		double side_x;
		double side_y;
		t_point step;
		if (raydir_x < 0)
		{
			step.x = -1;
			side_x = ((p.x) - map_x) * delta_x;
		}
		else
		{
			step.x = 1;
			side_x = (map_x + 1.0 - (p.x)) * delta_x;
		}
		if (raydir_y < 0)
		{
			step.y = -1;
			side_y = ((p.y) - map_y) * delta_y;
		}
		else
		{
			step.y = 1;
			side_y = (map_y + 1.0 - (p.y)) * delta_y;
		}

		bool hit = false;
		int side = -1; // -1 = no hit, 0 = hit vertical wall, 1 = hit horizontal wall
		while (!hit)
		{
			if (side_x < side_y)
			{
				side_x += delta_x;
				map_x += step.x;
				hit = lvl->map[map_y][map_x] == '1';
				side = 0;
			}
			else
			{
				side_y += delta_y;
				map_y += step.y;
				hit = lvl->map[map_y][map_x] == '1';
				side = 1;
			}
		}
		(void)side;

		drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){map_x * TILE_SIZE, map_y * TILE_SIZE}, 0xFFFFFFFF);

		// calculate the perpendicular distance to the wall
		double perp_wall_dist;
		if (side == 0)
			perp_wall_dist = (map_x - p.x + (1 - step.x) / 2) / raydir_x;
		else
			perp_wall_dist = (map_y - p.y + (1 - step.y) / 2) / raydir_y;

		// Calculate intersection point
		double hit_x = p.x + raydir_x * perp_wall_dist;
		double hit_y = p.y + raydir_y * perp_wall_dist;

		// Euclidean distance from player to intersection point
		// double euclidean_dist = sqrt((hit_x - p.x) * (hit_x - p.x) + (hit_y - p.y) * (hit_y - p.y));
		// drawline_angle(img, (t_point){p.x * PIXEL_SIZE, p.y * PIXEL_SIZE}, atan2(raydir_y, raydir_x), euclidean_dist, 0xFFd6ffcf);
		drawline(img, (t_point){p.x * PIXEL_SIZE, p.y * PIXEL_SIZE}, (t_point){hit_x * PIXEL_SIZE, hit_y * PIXEL_SIZE}, 0xFFd6ffcf);

		x += 20;
	}
}
