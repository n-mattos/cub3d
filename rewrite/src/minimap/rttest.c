/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rttest.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos- <nmattos-@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 12:58:15 by mschippe      #+#    #+#                 */
/*   Updated: 2025/07/14 14:32:48 by nmattos       ########   odam.nl         */
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

		// drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){map_x * TILE_SIZE, map_y * TILE_SIZE}, 0xFFFFFFFF);

		// calculate the perpendicular distance to the wall
		double perp_wall_dist;
		if (side == 0)
			perp_wall_dist = (map_x - p.x + (1 - step.x) / 2) / raydir_x;
		else
			perp_wall_dist = (map_y - p.y + (1 - step.y) / 2) / raydir_y;

		// Calculate intersection point
		double hit_x = p.x + raydir_x * perp_wall_dist;
		double hit_y = p.y + raydir_y * perp_wall_dist;

		drawline(img, (t_point){p.x * PIXEL_SIZE, p.y * PIXEL_SIZE}, (t_point){hit_x * PIXEL_SIZE, hit_y * PIXEL_SIZE}, 0xFFd6ffcf);

		x += (int)img->width / TOTAL_RAYS;
	}
}
