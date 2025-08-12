/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:59:36 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/12 13:41:43 by nmattos-         ###   ########.fr       */
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
	t_level	*level = d->level;
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
	raycast_dda(level, d->minimap, d->last_frame);
	mlx_set_mouse_pos(d->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
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
