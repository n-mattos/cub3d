/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:34:49 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/25 17:05:50 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static uint32_t	getcolor(t_tile tile);

/**
 * Draws the minimap based on the level map.
 * @param d Pointer to the game data structure containing the minimap and level.
 */
void	draw_minimap(t_data *d)
{
	int	y;
	int	x;

	y = 0;
	while (d->level->map[y])
	{
		x = 0;
		while (d->level->map[y][x])
		{
			//temp kinda fix, this check just cuts off map drawing when it knows a rect is gonna go out of bounds
			//ideally we do scaling instead,just a bit lazy rn
			if (((x - d->level->player->x) * RECT_SIZE < 200 && (y - d->level->player->y) * RECT_SIZE < 200)
				&& (x - d->level->player->x) * RECT_SIZE > -200 && (y - d->level->player->y) * RECT_SIZE > -200)
				drawrectangle(
					d->minimap, (t_point){RECT_SIZE, RECT_SIZE},
					(t_point){(x - d->level->player->x) * RECT_SIZE + 200, (y - d->level->player->y) * RECT_SIZE + 200},
					getcolor(d->level->map[y][x]));
			x++;
		}
		y++;
	}
}

/**
 * Draws the rays on the minimap.
 * @param mmap The minimap image to draw on.
 * @param p The player data containing the player's position.
 * @param intersect The intersection point of the ray.
 * @param x The x-coordinate of the ray.
 */
void	draw_minimap_rays(mlx_image_t *mmap, t_playerdata p,
				t_vect intersect, int x)
{
	if (x >= 0 && x <= (int)mmap->width - 1 && x % (100 / PERCENTAGE_RAYS) == 0)
	{
		drawline(
			mmap,
			// (t_point){p.x * RECT_SIZE, p.y * RECT_SIZE},
			(t_point){200, 200},
			// (t_point){(intersect.x * 200) / p.x, (intersect.y * 200) / p.y},
			(t_point){(intersect.x / p.x) * 200, (intersect.y / p.y) * 200},
			0xFFd6ffcf);
	}
}

/**
 * Retrieves the color for a given tile.
 * @param tile The tile type to get the color for.
 * @return The color associated with the tile type.
 */
static uint32_t	getcolor(t_tile tile)
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
