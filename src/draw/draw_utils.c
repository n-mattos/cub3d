/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:37:23 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/24 10:26:12 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	in_circle(t_point point, t_point center, int radius);
static void	drawline_draw(mlx_image_t *img, t_point a, t_point b,
				uint32_t color);

void	draw_circle_outline(mlx_image_t *img, t_point center, int radius, uint32_t color)
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

void	fill_circle(mlx_image_t *img, t_point center, int radius, uint32_t color)
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
 * * Draws a line between two points on the image.
 * @param img The image to draw on.
 * @param a The starting point of the line.
 * @param b The ending point of the line.
 * @param color The color of the line.
 */
void	drawline(mlx_image_t *img, t_point a, t_point b, uint32_t color)
{
	if (a.x == b.x && a.y == b.y)
	{
		mlx_put_pixel(img, a.x, a.y, color);
		return ;
	}
	drawline_draw(img, a, b, color);
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
			break;
		x = 0;
		y++;
	}
}

/**
 * Utility function for draw_line that performs the actual drawing of the line
 * @param img The image to draw on.
 * @param a The starting point of the line.
 * @param b The ending point of the line.
 * @param color The color of the line.
 */
static void	drawline_draw(mlx_image_t *img, t_point a, t_point b,
	uint32_t color)
{
	uint32_t	x;
	uint32_t	y;
	t_vect		inc;
	t_vect		pos;

	inc.x = ((double)b.x - (double)a.x) / LINESMOOTHNESS;
	inc.y = ((double)b.y - (double)a.y) / LINESMOOTHNESS;
	pos.x = ((double)a.x);
	pos.y = ((double)a.y);
	while ((fabs((double)b.x - pos.x) > 0.001
		|| fabs((double)b.y - pos.y) > 0.001))
	{
		x = (uint32_t)(pos.x + 0.5);
		y = (uint32_t)(pos.y + 0.5);
		if (in_circle((t_point){x, y},
				(t_point){MMAP_DIAM / 2, MMAP_DIAM / 2}, MMAP_DIAM / 2))
			mlx_put_pixel(img, x, y, color);
		pos.x += inc.x;
		pos.y += inc.y;
	}
}

static bool	in_circle(t_point point, t_point center, int radius)
{
	int	dx;
	int	dy;

	dx = point.x - center.x;
	dy = point.y - center.y;
	return (dx * dx + dy * dy <= radius * radius);
}

/**
 * Draws a vertical line in the image
 * from point a to point b with the specified color.
 * @param img Pointer to the image where the line will be drawn.
 * @param a First point defining the vertical line.
 * @param b Second point defining the vertical line.
 * @param color Color of the line to be drawn.
 */
void	drawvert(mlx_image_t *img, t_point a, t_point b, uint32_t color)
{
	int			y_start;
	int			y_end;
	int			y;
	int			x;

	if (a.x == b.x)
	{
		x = a.x;
		if (x < 0 || x >= (int)img->width)
			return ;
		y_start = fmin(a.y, b.y);
		y_end = fmax(a.y, b.y);
		if (y_start < 0)
			y_start = 0;
		if (y_end >= (int)img->height)
			y_end = img->height - 1;
		y = y_start;
		while (y <= y_end)
		{
			mlx_put_pixel(img, x, y, color);
			y++;
		}
	}
}
