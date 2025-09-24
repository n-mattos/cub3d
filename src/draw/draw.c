/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:59:36 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/24 12:37:03 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	draw_floor_ceiling(mlx_image_t *img, t_textures *textures);

/**
 * Draws the entire game frame, including the minimap and the main view.
 * @param data Pointer to the game data structure.
 */
void	draw_all(t_data *d)
{
	mlx_t	*mlx;

	mlx = d->mlx;
	if (!d->background)
	{
		d->background = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
		mlx_image_to_window(mlx, d->background, 0, 0);
		draw_floor_ceiling(d->background, d->level->textures);
	}
	if (!d->last_frame)
	{
		d->last_frame = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
		mlx_image_to_window(mlx, d->last_frame, 0, 0);
	}
	if (!d->minimap)
	{
		d->minimap = mlx_new_image(mlx, 400, 400);
		mlx_image_to_window(mlx, d->minimap, 0, 0);
	}
	draw_minimap(d);
	ft_memset(d->last_frame->pixels, 0, d->last_frame->width * d->last_frame->height * 4);
	raycast_dda(d);
	mlx_set_mouse_pos(d->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}

static void	draw_floor_ceiling(mlx_image_t *img, t_textures *textures)
{
	for (int y = 0; y < (int)img->height / 2; y++)
	{
		for (int x = 0; x < (int)img->width; x++)
		{
			mlx_put_pixel(img, x, y, textures->ceiling);
			mlx_put_pixel(img, x, img->height - y - 1, textures->floor);
		}
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
		if (x < IMG_WIDTH && y < IMG_HEIGHT && x >= 0 && y >= 0)
			mlx_put_pixel(d->last_frame, x, y,
				get_pixel_color(d->level->textures, ray));
		ray->txt_pos += step;
	}
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
