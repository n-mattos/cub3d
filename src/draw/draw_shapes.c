/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw_shapes.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos <nmattos@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/30 08:56:07 by nmattos       #+#    #+#                 */
/*   Updated: 2025/09/30 08:56:33 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	draw_circle_outline(mlx_image_t *img, t_point center,
			int radius, uint32_t color)
{
	int			x;
	int			y;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (in_circle((t_point){x + center.x, y + center.y},
				(t_point){center.x, center.y}, radius)
				&& !in_circle((t_point){x + center.x, y + center.y},
				(t_point){center.x, center.y}, radius - MMAP_BORDER_THICKNESS))
			{
				mlx_put_pixel(img, x + center.x, y + center.y, color);
			}
			x++;
		}
		y++;
	}
}

void	fill_circle(mlx_image_t *img, t_point center,
			int radius, uint32_t color)
{
	int			x;
	int			y;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (in_circle((t_point){x + center.x, y + center.y},
				(t_point){center.x, center.y}, radius))
			{
				mlx_put_pixel(img, x + center.x, y + center.y, color);
			}
			x++;
		}
		y++;
	}
}

/**
 * * Draws a rectangle on the image.
 * @param img The image to draw on.
 * @param wh The width and height of the rectangle.
 * @param coord The top-left corner coordinates of the rectangle.
 * @param color The color of the rectangle.
 */
void	drawrectangle(mlx_image_t *img, t_point wh,
			t_point coord, uint32_t color)
{
	int			x;
	int			y;

	x = MMAP_SQUARE_BORDER;
	y = MMAP_SQUARE_BORDER;
	while (y < wh.y - MMAP_SQUARE_BORDER)
	{
		while (x < wh.x - MMAP_SQUARE_BORDER)
		{
			if (in_circle((t_point){x + coord.x, y + coord.y},
				(t_point){MMAP_DIAM / 2, MMAP_DIAM / 2}, MMAP_DIAM / 2))
				mlx_put_pixel(img, x + coord.x, y + coord.y, color);
			x++;
		}
		if (!(y + coord.y < IMG_WIDTH && x + coord.x < IMG_HEIGHT))
			break ;
		x = 0;
		y++;
	}
}
