/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/12 12:46:58 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	set_start_end(int *y_start, int *y_end, t_point a, t_point b, mlx_image_t *img);
static void	drawvert(mlx_image_t *img, t_point a, t_point b, uint32_t color);
static void	draw_wall(mlx_image_t *img, double perp_dist, int side, int x);

/**
 * Performs the DDA (Digital Differential Analyzer) algorithm for raycasting.
 * @param lvl Pointer to the game level structure containing map, textures, and player data.
 * @param mmap Pointer to the minimap image where rays will be drawn.
 * @param frame Pointer to the frame image where walls will be drawn.
 */
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
		draw_minimap_rays(mmap, p, intersect, x);
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

/**
 * Sets the start and end points for drawing a vertical line in the image.
 * @param y_start Pointer to the start y-coordinate.
 * @param y_end Pointer to the end y-coordinate.
 * @param a First point defining the vertical line.
 * @param b Second point defining the vertical line.
 * @param img Pointer to the image where the line will be drawn.
 */
static void	set_start_end(int *y_start, int *y_end, t_point a, t_point b, mlx_image_t *img)
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

/**
 * Draws a vertical line in the image from point a to point b with the specified color.
 * @param img Pointer to the image where the line will be drawn.
 * @param a First point defining the vertical line.
 * @param b Second point defining the vertical line.
 * @param color Color of the line to be drawn.
 */
static void	drawvert(mlx_image_t *img, t_point a, t_point b, uint32_t color)
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

/**
 * Draws a wall segment in the frame image based on the perpendicular distance and side hit.
 * @param img Pointer to the frame image where the wall will be drawn.
 * @param perp_dist Perpendicular distance to the wall.
 * @param side Side of the wall hit (0 for vertical, 1 for horizontal).
 * @param x X-coordinate of the column where the wall is drawn.
 */
static void	draw_wall(mlx_image_t *img, double perp_dist, int side, int x)
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
