/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:34:49 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 11:20:38 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static uint32_t	getcolor(char tile);

/**
 * Draws the minimap based on the level map.
 * @param d Pointer to the game data structure containing the minimap and level.
 */
void	draw_minimap(t_data *d)
{
	int	y;
	int	x;

	fill_circle(d->minimap, (t_point){MMAP_DIAM / 2, MMAP_DIAM / 2}, MMAP_DIAM / 2, EMPTY_COLOR);
	y = 0;
	while (d->level->map[y])
	{
		x = 0;
		while (d->level->map[y][x])
		{
			if ((MMAP_DIAM / 2 + (x - d->level->player->x) * d->rect <= MMAP_DIAM + d->rect && MMAP_DIAM / 2 + (y - d->level->player->y) * d->rect <= MMAP_DIAM + d->rect)
				&& MMAP_DIAM / 2 + (x - d->level->player->x) * d->rect > 0 - d->rect && MMAP_DIAM / 2 + (y - d->level->player->y) * d->rect > 0 - d->rect)
				drawrectangle(
					d->minimap, (t_point){d->rect, d->rect},
					(t_point){MMAP_DIAM / 2 + (x - d->level->player->x) * d->rect, MMAP_DIAM / 2 + (y - d->level->player->y) * d->rect},
					getcolor(d->level->map[y][x]));
			x++;
		}
		y++;
	}
	draw_circle_outline(d->minimap, (t_point){MMAP_DIAM / 2, MMAP_DIAM / 2}, MMAP_DIAM / 2, 0xFFFFFFFF);
}

/**
 * Draws the rays on the minimap.
 * @param mmap The minimap image to draw on.
 * @param p The player data containing the player's position.
 * @param intersect The intersection point of the ray.
 * @param x The x-coordinate of the ray.
 */
void	draw_minimap_rays(t_data *d, t_playerdata p,
				t_vect intersect, int x)
{
	if (x >= 0 && x <= (int)IMG_WIDTH - 1 && x % (100 / PERCENTAGE_RAYS) == 0)
	{
		drawline(
			d->minimap,
			(t_point){MMAP_DIAM / 2, MMAP_DIAM / 2},
			(t_point){MMAP_DIAM / 2 + (intersect.x - p.x) * d->rect, MMAP_DIAM / 2 + (intersect.y - p.y) * d->rect},
			RED);
	}
}

static uint32_t	get_portal_color(char c)
{
	int	r;
	int	g;
	int	b;

	r = (c * 47 + 85) % 128 + 128;
	g = (c * 67 + 29) % 128 + 128;
	b = (c * 89 + 123) % 128 + 128;
	return (r << 24) | (g << 16) | (b << 8) | 255;
}
/**
 * Retrieves the color for a given tile.
 * @param tile The tile type to get the color for.
 * @return The color associated with the tile type.
 */
static uint32_t	getcolor(char tile)
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
		return (get_portal_color(tile));
}
