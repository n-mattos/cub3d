/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/08/12 17:26:33 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

	while (x < (int)frame->width)
	{
		raydir = calculate_raydir(frame, p, x);
		delta = calculate_delta(raydir);
		map = calculate_map(p);
		side = calculate_side(p, raydir, map, delta, &step);

		hit_side = -1; // -1 = no hit, 0 = hit vertical wall, 1 = hit horizontal wall
		calculate_ray(&map, &side, delta, step, &hit_side, lvl);

		perp_wall_dist = calculate_perpendicular_distance(p, raydir, map, hit_side, step);
		intersect = calculate_intersection(p, raydir, perp_wall_dist);
		draw_minimap_rays(mmap, p, intersect, x);



		double wall_x;
		if (hit_side == 0)
			wall_x = p.y + perp_wall_dist * raydir.y;
		else
			wall_x = p.x + perp_wall_dist * raydir.x;
		wall_x -= (int)wall_x;

		int txt_x = (int)(wall_x * TEXTURE_WIDTH);
		if ((hit_side == 0 && raydir.x > 0) || (hit_side == 1 && raydir.y < 0))
			txt_x = TEXTURE_WIDTH - txt_x - 1;

		uint32_t *pixels = (uint32_t *)frame->pixels;

		int			line_height;
		int			draw_start;
		int			draw_end;
		uint32_t	color;

		line_height = (int)(IMG_HEIGHT / perp_wall_dist);
		double step = 1.0 * TEXTURE_HEIGHT / line_height;
		draw_start = -line_height / 2 + IMG_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = line_height / 2 + IMG_HEIGHT / 2;
		if (draw_end >= IMG_HEIGHT)
			draw_end = IMG_HEIGHT - 1;

		double txt_pos = (draw_start - IMG_HEIGHT / 2 + line_height / 2) * step;
		for (int y = draw_start; y < draw_end; y++)
		{
			int txt_y = (int)txt_pos & (TEXTURE_HEIGHT - 1);
			color = get_pixel_color(lvl->textures->wall, txt_x, txt_y);
			mlx_put_pixel(frame, x, y, color);
			txt_pos += step;
		}

		x += (int)frame->width / TOTAL_RAYS;
	}
}
