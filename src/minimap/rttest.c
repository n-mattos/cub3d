/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rttest.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/06/25 12:12:44 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/minimap.h"

#define PI 3.14159265358979323846
#define EPSILON 1.e-10
#define TILE_SIZE 40
#define PIXEL_SIZE 1000000

t_point	findplayer(t_level *level)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (level && level->map[y])
	{
		while (level->map[y][x])
		{
			if (is_player(level->map[y][x]))
				return ((t_point){x * PIXEL_SIZE, y * PIXEL_SIZE});
			x++;
		}
		x = 0;
		y++;
	}
	return ((t_point){-1, -1});
}

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
	printf("a: %u, %u b: %u, %u\n", a.x, a.y, b.x, b.y);
	if (a.x == b.x && a.y == b.y)
	{
		pixels[a.y * img->width + a.x] = color;
		return;
	}
	double	dx = (double)((double)b.x - (double)a.x) / PIXEL_SIZE;
	double	dy = (double)((double)b.y - (double)a.y) / PIXEL_SIZE;
	printf("dx: %f, dy: %f\n", dx, dy);
	double	x_inc = dx / 10000; // bigger division => smoother line; more iterations
	double	y_inc = dy / 10000;
	printf("x_inc: %f, y_inc: %f\n", x_inc, y_inc);
	double	x_pos = (a.x / PIXEL_SIZE) * TILE_SIZE;
	double	y_pos = (a.y / PIXEL_SIZE) * TILE_SIZE;
	printf("Drawing line from: %f, %f to %f, %f\n", x_pos, y_pos, (double)b.x / PIXEL_SIZE * TILE_SIZE, (double)b.y / PIXEL_SIZE * TILE_SIZE);
	printf("while: %f, %f\n", (double)b.x / PIXEL_SIZE * TILE_SIZE - x_pos, (double)b.y / PIXEL_SIZE * TILE_SIZE - y_pos);
	while (fabs((double)b.x / PIXEL_SIZE * TILE_SIZE - x_pos) > 0.001 || fabs((double)b.y / PIXEL_SIZE * TILE_SIZE - y_pos) > 0.001)
	{
		if (x_pos < 0 || y_pos < 0 || x_pos >= img->width || y_pos >= img->height)
			break;
		x = (uint32_t)(x_pos + 0.5);
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
	t_point player = findplayer(lvl);
	if (lvl->map[player.y / PIXEL_SIZE][player.x / PIXEL_SIZE] == NORTH)
		angle_deg -= 90.0;
	else if (lvl->map[player.y / PIXEL_SIZE][player.x / PIXEL_SIZE] == EAST)
		angle_deg -= 0.0;
	else if (lvl->map[player.y / PIXEL_SIZE][player.x / PIXEL_SIZE] == SOUTH)
		angle_deg -= 270.0;
	else if (lvl->map[player.y / PIXEL_SIZE][player.x / 100000] == WEST)
		angle_deg -= 180.0;
	if (angle_deg < 0)
			angle_deg += 360;
	double angle_rad = angle_deg * PI / 180.0;
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
	step = (t_point){raydir_x * PIXEL_SIZE, raydir_y * PIXEL_SIZE};
	while (!hit)
	{
		player.x += step.x;
		player.y += step.y;
		printf("coord check: %d,%d\n", player.x / PIXEL_SIZE, player.y / PIXEL_SIZE);
		hit = lvl->map[player.y / PIXEL_SIZE][player.x / PIXEL_SIZE] == '1';
	}
	drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){player.x / PIXEL_SIZE * TILE_SIZE, player.y / PIXEL_SIZE * TILE_SIZE}, 0xFFFFFFFF);
	drawline(img, findplayer(lvl), (t_point){player.x, player.y}, 0xFFFF00FF);
	printf("Hit wall at %d,%d\n", player.x / PIXEL_SIZE, player.y / PIXEL_SIZE);
}
