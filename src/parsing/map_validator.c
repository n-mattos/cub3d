/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:43 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/12 12:29:48 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	invalid_tile(int **map, size_t y, size_t x);

/**
 * Checks if the map is valid.
 * A valid map must have exactly one player, and be surrounded by walls.
 * @param map The map to validate.
 * @return true if the map is valid, false otherwise.
 */
bool	map_is_valid(int **map)
{
	size_t	player;
	size_t	x;
	size_t	y;

	y = 0;
	player = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != '\0')
		{
			if (is_player(map[y][x]))
				player++;
			if (invalid_tile(map, y, x))
				return (false);
			x++;
		}
		y++;
	}
	if (player != 1)
		return (false);
	return (true);
}

/**
 * Checks if a tile is invalid.
 * @param map The map to check.
 * @param y The y coordinate of the tile.
 * @param x The x coordinate of the tile.
 * @return true if the tile is invalid, false otherwise.
 */
static bool	invalid_tile(int **map, size_t y, size_t x)
{
	if (map[y][x] == EMPTY || map[y][x] == WALL)
		return (false);
	if (x == 0 || map[y][x] == '\0' || map[y][x + 1] == '\0' ||
		y == 0 || map[y + 1] == NULL)
		return (true);
	if (map[y + 1][x] == EMPTY ||
		map[y - 1][x] == EMPTY ||
		map[y][x + 1] == EMPTY ||
		map[y][x - 1] == EMPTY)
		return (true);
	return (false);
}
