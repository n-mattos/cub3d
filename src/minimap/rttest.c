/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rttest.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/10 13:48:58 by nmattos-         ###   ########.fr       */
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
	double	x_inc = dx / 10000; // bigger division => smoother line; more iterations
	double	y_inc = dy / 10000;
	double	x_pos = ((double)a.x / PIXEL_SIZE) * TILE_SIZE;
	double	y_pos = ((double)a.y / PIXEL_SIZE) * TILE_SIZE;
	while (fabs((double)b.x / PIXEL_SIZE * TILE_SIZE - x_pos) > 0.001 || fabs((double)b.y / PIXEL_SIZE * TILE_SIZE - y_pos) > 0.001)
	{
		if (x_pos < 0 || y_pos < 0 || x_pos >= img->width || y_pos >= img->height)
			break;
		x = (uint32_t)(x_pos + 0.5);	// + 0.5 to round to the nearest integer
		y = (uint32_t)(y_pos + 0.5);
		if (x < img->width && y < img->height)
			pixels[y * img->width + x] = color;
		x_pos += x_inc;
		y_pos += y_inc;
	}
}
/*
void	single_ray_slow(t_level *lvl, mlx_image_t *img, double angle_deg)
{
	// double delta_x;
	// double delta_y;

	t_playerdata player = *lvl->player;
	// double angle_rad = (angle_deg + player.degrees) * PI / 180.0;
	double raydir_x = cos(angle_rad);
	double raydir_y = -sin(angle_rad);
	t_point step;
	// if (fabs(raydir_x) < EPSILON)
	// 	delta_x = 0;
	// else
	// 	delta_x = fabs(1 / raydir_x);
	// if (fabs(raydir_y) < EPSILON)
	// 	delta_y = 0;
	// else
	// 	delta_y = fabs(1 / raydir_y);
	bool hit = false;
	step = (t_point){raydir_x * PIXEL_SIZE / 10, raydir_y * PIXEL_SIZE / 10};	// bigger step size => faster raycasting, but less accurate
	player.x = player.x + PIXEL_SIZE / 2;
	player.y = player.y + PIXEL_SIZE / 2;
	t_point vector = (t_point){player.x, player.y};
	while (!hit)
	{
		player.x += step.x;
		player.y += step.y;
		hit = lvl->map[player.y / PIXEL_SIZE][player.x / PIXEL_SIZE] == '1';
	}
	drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){player.x / PIXEL_SIZE * TILE_SIZE, player.y / PIXEL_SIZE * TILE_SIZE}, 0xFFFFFFFF);
	drawline(img, vector, (t_point){player.x, player.y}, 0xFFFF00FF);
}
	*/

void	single_ray(t_level *lvl, mlx_image_t *img, double angle_deg)
{
	t_playerdata player = *lvl->player;
	(void)angle_deg;
	// double deg = (angle_deg + player.degrees);
	// if (deg < 0)
	// 	deg += 360;
	// else if (deg > 360)
	// 	deg -= 360;
	// double angle_rad = deg * PI / 180.0;
	// double raydir_x = cos(angle_rad);
	// double raydir_y = -sin(angle_rad);
	// printf("Ray angle: %f degrees\n", deg);
	// printf("Ray direction: (%f, %f)\n", raydir_x, raydir_y);

	// double dir_x = -1;
	// double dir_y = 0;
	// double dir_x = cos(deg * PI / 180.0);
	// double dir_y = -sin(deg * PI / 180.0);
	// double plane_x = 0;
	// double plane_y = 0.66;
	// plane_x = dir_y * 0.66;
	// plane_y = -dir_x * 0.66;
	// printf("Direction: (%f, %f)\n", dir_x, dir_y);
	// printf("Plane: (%f, %f)\n", plane_x, plane_y);

	// for (int x = 0; x < (int)img->width - 100; x++)
	// {
		int x = img->width / 2;
		double camera_x = 2.0 * x / (double)img->width - 1.0;
		double raydir_x = player.dir_x + player.plane_x * camera_x;
		double raydir_y = player.dir_y + player.plane_y * camera_x;

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

		// Use local variables for ray position
		double ray_pos_x = player.x + PIXEL_SIZE / 2;
		double ray_pos_y = player.y + PIXEL_SIZE / 2;
		int map_x = ray_pos_x / PIXEL_SIZE;
		int map_y = ray_pos_y / PIXEL_SIZE;
		double side_x;
		double side_y;
		t_point step;
		if (raydir_x < 0)
		{
			step.x = -1;
			side_x = ((ray_pos_x / PIXEL_SIZE) - map_x) * delta_x;
		}
		else
		{
			step.x = 1;
			side_x = (map_x + 1.0 - (ray_pos_x / PIXEL_SIZE)) * delta_x;
		}
		if (raydir_y < 0)
		{
			step.y = -1;
			side_y = ((ray_pos_y / PIXEL_SIZE) - map_y) * delta_y;
		}
		else
		{
			step.y = 1;
			side_y = (map_y + 1.0 - (ray_pos_y / PIXEL_SIZE)) * delta_y;
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
			drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){map_x * TILE_SIZE, map_y * TILE_SIZE}, 0xFFFFFFFF);
		}
		(void)side;

		drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){map_x * TILE_SIZE, map_y * TILE_SIZE}, 0xFFFFFFFF);
		drawline(img, (t_point){ray_pos_x, ray_pos_y}, (t_point){map_x * PIXEL_SIZE, map_y * PIXEL_SIZE}, 0xFFFF00FF);
	// }
}
