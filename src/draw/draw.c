/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:59:36 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/02 17:18:12 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	draw_portal_effect(t_data *d);
static bool	draw_pixel(t_data *d, t_raycast *ray, int x, int y);

/**
 * Draws the entire game frame, including the minimap and the main view.
 * @param data Pointer to the game data structure.
 */
void	draw_all(t_data *d)
{
	create_background(d);
	create_last_frame(d);
	ft_memset(d->last_frame->pixels, 0,
		d->last_frame->width * d->last_frame->height * 4);
	create_minimap(d);
	draw_minimap(d);
	raycast_dda(d);
	create_portal_effect(d);
	draw_portal_effect(d);
	create_crosshair(d);
	mlx_set_mouse_pos(d->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}

static void	draw_portal_effect(t_data *d)
{
	int	i;

	i = 0;
	if (d->level->portal_effect_opacity > 0x00)
		d->level->portal_effect_opacity -= 3;
	if (d->level->portal_effect_opacity < 0x00)
		d->level->portal_effect_opacity = 0x00;
	while (i < IMG_WIDTH * IMG_HEIGHT * 4)
	{
		d->portal_effect->pixels[i] = 0x00;
		d->portal_effect->pixels[i + 1] = 0xFF;
		d->portal_effect->pixels[i + 2] = 0x00;
		d->portal_effect->pixels[i + 3] = d->level->portal_effect_opacity;
		i += 4;
	}
}

void	draw_textured_wall(t_raycast *ray, t_data *d, int x)
{
	double	step;
	int		y;

	ray->txt_x = (int)(calculate_wallx(d->level->player, ray->hit_side,
				ray->perp_wall_dist, ray->raydir) * TEXTURE_WIDTH);
	if ((ray->hit_side == VERTICAL && ray->raydir.x > 0)
		|| (ray->hit_side == HORIZONTAL && ray->raydir.y < 0))
		ray->txt_x = TEXTURE_WIDTH - ray->txt_x - 1;
	ray->line_height = (int)(IMG_HEIGHT / ray->perp_wall_dist);
	step = 1.0 * TEXTURE_HEIGHT / ray->line_height;
	ray->draw_start = -ray->line_height / 2 + IMG_HEIGHT / 2;
	ray->draw_end = ray->line_height / 2 + IMG_HEIGHT / 2;
	if (ray->draw_end >= IMG_HEIGHT)
		ray->draw_end = IMG_HEIGHT - 1;
	ray->txt_pos = (ray->draw_start - IMG_HEIGHT / 2 + ray->line_height / 2)
		* step;
	y = ray->draw_start - 1;
	while (++y < ray->draw_end)
	{
		ray->txt_y = (int)ray->txt_pos & (TEXTURE_HEIGHT - 1);
		if (draw_pixel(d, ray, x, y) == 1)
			break ;
		ray->txt_pos += step;
	}
}

void	draw_door(t_raycast *ray, t_data *d, int x)
{
	double	step;
	int		y;

	ray->txt_x = (int)(calculate_wallx(d->level->player, ray->hit_side,
				ray->perp_wall_dist, ray->raydir) * TEXTURE_WIDTH);
	if ((ray->hit_side == VERTICAL && ray->raydir.x > 0)
		|| (ray->hit_side == HORIZONTAL && ray->raydir.y < 0))
		ray->txt_x = TEXTURE_WIDTH - ray->txt_x - 1;
	ray->line_height = (int)(IMG_HEIGHT / ray->perp_wall_dist);
	step = 1.0 * TEXTURE_HEIGHT / ray->line_height;
	ray->draw_start = -ray->line_height / 2 + IMG_HEIGHT / 2;
	ray->draw_end = ray->line_height / 2 + IMG_HEIGHT / 2;
	if (ray->draw_end >= IMG_HEIGHT)
		ray->draw_end = IMG_HEIGHT - 1;
	ray->txt_pos = (ray->draw_start - IMG_HEIGHT / 2 + ray->line_height / 2)
		* step;
	y = ray->draw_start - 1;
	while (++y < ray->draw_end)
	{
		ray->txt_y = (int)ray->txt_pos & (TEXTURE_HEIGHT - 1);
		uint32_t color = get_pixel_color(d->level->textures, ray);
		if (color == 0x00000000)
			continue ;
		if (x < IMG_WIDTH && y < IMG_HEIGHT && x >= 0 && y >= 0)
			mlx_put_pixel(d->last_frame, x, y, color);
		ray->txt_pos += step;
	}
}

static bool	draw_pixel(t_data *d, t_raycast *ray, int x, int y)
{
	uint32_t	color;

	color = get_pixel_color(d->level->textures, ray);
	if (color == 0x00000000)
	{
		calculate_ray((&ray->map), ray, d->level, false);
		ray->perp_wall_dist = calculate_perpendicular_distance(*d->level->player, ray, ray->map);
		draw_textured_wall(ray, d, x);
		if (x < IMG_WIDTH && y < IMG_HEIGHT && x >= 0 && y >= 0)
			mlx_put_pixel(d->last_frame, x, y, color);
		return (1);
	}
	if (x < IMG_WIDTH && y < IMG_HEIGHT && x >= 0 && y >= 0)
		mlx_put_pixel(d->last_frame, x, y, color);
	return (0);
}

/**
 * Draws a wall segment in the frame image
 * based on the perpendicular distance and side hit.
 * @param img Pointer to the frame image where the wall will be drawn.
 * @param perp_dist Perpendicular distance to the wall.
 * @param side Side of the wall hit (0 for vertical, 1 for horizontal).
 * @param x X-coordinate of the column where the wall is drawn.
 */
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
	if (side == 0)
		color = 0xFF00FFFF;
	else
		color = 0xFFFF00FF;
	drawvert(img, (t_point){x, draw_start}, (t_point){x, draw_end}, color);
}
