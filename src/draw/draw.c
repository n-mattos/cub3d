/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:59:36 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/18 13:41:19 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Draws the entire game frame, including the minimap and the main view.
 * @param data Pointer to the game data structure.
 */
void	draw_all(void *data)
{
	t_data	*d;

	d = (t_data *)data;
	mlx_t	*mlx = d->mlx;
	if (!d->last_frame)
	{
		d->last_frame = mlx_new_image(mlx, 1920, 1080);
		mlx_image_to_window(mlx, d->last_frame, 0, 0);
	}
	if (!d->minimap)
	{
		d->minimap = mlx_new_image(mlx, 1920, 1080);
		mlx_image_to_window(mlx, d->minimap, 0, 0);
	}
	ft_memset(d->last_frame->pixels, 0, d->last_frame->width * d->last_frame->height * 4);
	ft_memset(d->minimap->pixels, 0, d->minimap->width * d->minimap->height * 4);
	draw_minimap(d);
	raycast_dda(d);
	mlx_set_mouse_pos(d->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}

void	draw_textured_wall(t_raycast *ray, t_data *d, int x)
{
	double	step;

	ray->txt_x = (int)(calculate_wallx(d->level->player, ray->hit_side, ray->perp_wall_dist, ray->raydir) * TEXTURE_WIDTH);
	if ((ray->hit_side == VERTICAL && ray->raydir.x > 0) || (ray->hit_side == HORIZONTAL && ray->raydir.y < 0))
		ray->txt_x = TEXTURE_WIDTH - ray->txt_x - 1;

	ray->line_height = (int)(IMG_HEIGHT / ray->perp_wall_dist);
	step = 1.0 * TEXTURE_HEIGHT / ray->line_height;
	ray->draw_start = -ray->line_height / 2 + IMG_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + IMG_HEIGHT / 2;
	if (ray->draw_end >= IMG_HEIGHT)
		ray->draw_end = IMG_HEIGHT - 1;

	ray->txt_pos = (ray->draw_start - IMG_HEIGHT / 2 + ray->line_height / 2) * step;
	for (int y = ray->draw_start; y < ray->draw_end; y++)
	{
		ray->txt_y = (int)ray->txt_pos & (TEXTURE_HEIGHT - 1);
		mlx_put_pixel(d->last_frame, x, y, get_pixel_color(d->level->textures->wall, ray->txt_x, ray->txt_y));
		ray->txt_pos += step;
	}
}

/**
 * Draws a wall segment in the frame image based on the perpendicular distance and side hit.
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
	if (side == 0) // Colors temporary, we will replace these with textures
		color = 0xFF00FFFF;
	else
		color = 0xFFFF00FF;
	drawvert(img, (t_point){x, draw_start}, (t_point){x, draw_end}, color);
}
