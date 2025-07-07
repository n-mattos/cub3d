/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rttest.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos- <nmattos-@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 12:58:15 by mschippe      #+#    #+#                 */
/*   Updated: 2025/07/07 12:52:40 by nmattos       ########   odam.nl         */
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

void	single_ray_slow(t_level *lvl, mlx_image_t *img, double angle_deg)
{
	// double delta_x;
	// double delta_y;

	t_playerdata player = *lvl->player;
	double angle_rad = (angle_deg + player.degrees) * PI / 180.0;
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

void	single_ray(t_level *lvl, mlx_image_t *img, double angle_deg)
{
	t_playerdata player = *lvl->player;
	double deg = (angle_deg + player.degrees);
	if (deg < 0)
		deg += 360;
	else if (deg >= 360)
		deg -= 360;
	// double angle_rad = deg * PI / 180.0;
	// double raydir_x = cos(angle_rad);
	// double raydir_y = -sin(angle_rad);
	printf("Ray angle: %f degrees\n", deg);
	// printf("Ray direction: (%f, %f)\n", raydir_x, raydir_y);

	double dir_x = 0;
	double dir_y = 0;
	double plane_x = 0;
	double plane_y = 0.66;
	double old_dir_x = dir_x;
	dir_x = dir_x * cos(deg * PI / 180.0) + dir_y * sin(deg * PI / 180.0);
	dir_y = -old_dir_x * sin(deg * PI / 180.0) + dir_y * cos(deg * PI / 180.0);

	double old_plane_x = plane_x;
	plane_x = plane_x * cos(deg * PI / 180.0) + plane_y * sin(deg * PI / 180.0);
	plane_y = -old_plane_x * sin(deg * PI / 180.0) + plane_y * cos(deg * PI / 180.0);

	// for (int x = 0; x < (int)img->width; x++)
	// {
	int x = 1;
	double camera_x = 2 * x / (double)img->width - 1;
	double raydir_x = dir_x + plane_x * camera_x;
	double raydir_y = dir_y + plane_y * camera_x;


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


	int map_x = player.x / PIXEL_SIZE;
	int map_y = player.y / PIXEL_SIZE;
	double side_x;
	double side_y;
	t_point step;
	if (raydir_x < 0)
	{
		step.x = -1;
		side_x = ((player.x / PIXEL_SIZE) - map_x) * delta_x;
	}
	else
	{
		step.x = 1;
		side_x = (map_x + 1.0 - (player.x / PIXEL_SIZE)) * delta_x;
	}
	if (raydir_y < 0)
	{
		step.y = -1;
		side_y = ((player.y / PIXEL_SIZE) - map_y) * delta_y;
	}
	else
	{
		step.y = 1;
		side_y = (map_y + 1.0 - (player.y / PIXEL_SIZE)) * delta_y;
	}


	bool hit = false;
	player.x = player.x + PIXEL_SIZE / 2;
	player.y = player.y + PIXEL_SIZE / 2;
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
	// printf("Hit at %d, %d\n Side: %d\n", map_x, map_y, side);
	(void)side;

	drawrectangle(img, (t_point){TILE_SIZE, TILE_SIZE}, (t_point){map_x * TILE_SIZE, map_y * TILE_SIZE}, 0xFFFFFFFF);
	drawline(img, (t_point){player.x, player.y}, (t_point){map_x * PIXEL_SIZE, map_y * PIXEL_SIZE}, 0xFFFF00FF);
	// }
}
