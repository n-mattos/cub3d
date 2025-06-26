/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rttest.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/06/26 14:05:30 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/minimap.h"

#define PI 3.14159265358979323846
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
	// printf("Drawing line from: %f, %f to %f, %f\n", x_pos, y_pos, (double)b.x / PIXEL_SIZE * TILE_SIZE, (double)b.y / PIXEL_SIZE * TILE_SIZE);
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

void	single_ray(t_level *lvl, mlx_image_t *img, double angle_deg)
{
	double delta_x;
	double delta_y;

	t_playerdata player = *lvl->player;
	player.x *= PIXEL_SIZE;
	player.y *= PIXEL_SIZE;
	printf("Player pos: %lu,%lu\n", player.x / PIXEL_SIZE, player.y / PIXEL_SIZE);
	double angle_rad = (angle_deg + player.degrees) * PI / 180.0;
	double raydir_x = cos(angle_rad);
	double raydir_y = -sin(angle_rad);
	t_point step;
	if (fabs(raydir_x) < EPSILON)
		delta_x = 0;
	else
		delta_x = fabs(1 / raydir_x);
	if (fabs(raydir_y) < EPSILON)
		delta_y = 0;
	else
		delta_y = fabs(1 / raydir_y);
	bool hit = false;
	step = (t_point){raydir_x * PIXEL_SIZE / 100, raydir_y * PIXEL_SIZE / 100};	// bigger step size => faster raycasting, but less accurate
	player.x = player.x + PIXEL_SIZE / 2;
	player.y = player.y + PIXEL_SIZE / 2;
	t_point vector = (t_point){player.x, player.y};
	printf("Ray angle: %f degrees, %f radians\n", angle_deg, angle_rad);
	printf("Ray direction: %f, %f\n", raydir_x, raydir_y);
	printf("Delta X: %f, Delta Y: %f\n", delta_x, delta_y);
	printf("Step: %u, %u\n", step.x, step.y);
	printf("Vector: %u, %u\n", vector.x / PIXEL_SIZE, vector.y / PIXEL_SIZE);
	while (!hit)
	{
		player.x += step.x;
		player.y += step.y;
		printf("coord check: %lu,%lu\n", player.x / PIXEL_SIZE, player.y / PIXEL_SIZE);
		hit = lvl->map[player.y / PIXEL_SIZE][player.x / PIXEL_SIZE] == '1';
	}
	drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){player.x / PIXEL_SIZE * TILE_SIZE, player.y / PIXEL_SIZE * TILE_SIZE}, 0xFFFFFFFF);
	drawline(img, vector, (t_point){player.x, player.y}, 0xFFFF00FF);
	// printf("Hit wall at %d,%d\n", player.x / PIXEL_SIZE, player.y / PIXEL_SIZE);
}
