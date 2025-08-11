/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:34:49 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/11 13:59:09 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static uint32_t getcolor(t_tile tile)
{
	if (tile == FLOOR)
		return (FLOOR_COLOR);
	if (tile == EMPTY)
		return (EMPTY_COLOR);
	if (tile == WALL)
		return (WALL_COLOR);
	if (tile == NORTH || tile == EAST || tile == SOUTH || tile == WEST)
		return (FLOOR_COLOR);
	else
		return (0x12345678);
}

void	draw_minimap(t_data *d)
{
	int	y;

	y = 0;
	while (d->level->map[y])
	{
		int x = 0;
		while (d->level->map[y][x])
		{
			drawrectangle(
				d->minimap, (t_point){RECT_SIZE, RECT_SIZE},
				(t_point){x * RECT_SIZE, y * RECT_SIZE},
				getcolor(d->level->map[y][x]));
			x++;
		}
		y++;
	}
}

void	draw_minimap_rays(mlx_image_t *mmap, t_playerdata p, t_vect intersect, int x)
{
	if (x >= 0 && x <= (int)mmap->width - 1 && x % (100 / PERCENTAGE_RAYS) == 0)
	{
			drawline(
				mmap,
				(t_point){p.x * RECT_SIZE, p.y * RECT_SIZE},
				(t_point){intersect.x * RECT_SIZE, intersect.y * RECT_SIZE},
				0xFFd6ffcf);
	}
}
