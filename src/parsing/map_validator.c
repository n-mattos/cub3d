/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:43 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 12:32:18 by nmattos-         ###   ########.fr       */
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

t_point	get_portal_target(int **map, int x, int y)
{
	if (map[y + 1][x] == FLOOR)
		return ((t_point){x, y + 1});
	if (map[y - 1][x] == FLOOR)
		return ((t_point){x, y - 1});
	if (map[y][x + 1] == FLOOR)
		return ((t_point){x + 1, y});
	if (map[y][x - 1] == FLOOR)
		return ((t_point){x - 1, y});
	return (INVALID_POINT);
}

bool	is_valid_point(t_point point)
{
	return (point.x != -1 && point.y != -1);
}

bool	get_portals(int **map, t_portal_list **portals)
{
	size_t	x;
	size_t	y;
	t_portal_list	*found_tp;
	int		curr_tp_char;

	x = 0;
	y = 0;
	curr_tp_char = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != '\0')
		{
			if (map[y][x] != FLOOR && map[y][x] != WALL && map[y][x] != EMPTY && !is_player(map[y][x]) && map[y][x] != DOOR)
			{
				found_tp = find_portal_node(*portals, map[y][x]);
				if (!is_valid_point(get_portal_target(map, x, y)))
					return (free_portal_list(portals), false);
				else if (!found_tp)
					append_portal_node(portals, create_portal_node(map[y][x], (t_point){x, y}, get_portal_target(map, x, y)));
				else if(found_tp->B[SOURCE].x == -1)
					update_portal_node(found_tp, (t_point){x, y}, get_portal_target(map, x, y));
				else
					return (free_portal_list(portals), false);
			}
			x++;
		}
		y++;
	}
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
	if (map[y][x] != FLOOR)
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
