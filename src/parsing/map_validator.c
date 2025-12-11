/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:43 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/11 15:57:37 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool		fetch_portal(int **map, t_portal_list **portals,
					size_t x, size_t y);
static t_point	get_portal_target(int **map, int x, int y);
static bool		fetch_door(int **map, t_door_list **doors,
					size_t x, size_t y);
static bool		invalid_tile(int **map, size_t y, size_t x);

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

bool	get_portals(int **map, t_portal_list **portals)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != '\0')
		{
			if (!fetch_portal(map, portals, x, y))
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}

static bool	fetch_portal(int **map, t_portal_list **portals,
			size_t x, size_t y)
{
	t_portal_list	*found_tp;

	if (map[y][x] != FLOOR && map[y][x] != WALL
		&& map[y][x] != EMPTY && map[y][x] != DOOR
		&& !is_player(map[y][x]))
	{
		found_tp = find_portal_node(*portals, map[y][x]);
		if (get_portal_target(map, x, y).x == -1
			&& get_portal_target(map, x, y).y == -1)
			return (free_portal_list(portals), false);
		else if (!found_tp)
		{
			if (!append_portal_node(portals, create_portal_node(
						map[y][x], (t_point){x, y},
				get_portal_target(map, x, y))))
				return (free_portal_list(portals), false);
		}
		else if (found_tp->b[SOURCE].x == -1)
			update_portal_node(found_tp, (t_point){x, y},
				get_portal_target(map, x, y));
		else
			return (free_portal_list(portals), false);
	}
	return (true);
}

static t_point	get_portal_target(int **map, int x, int y)
{
	if (map[y + 1] != NULL && map[y + 1][x] == FLOOR)
		return ((t_point){x, y + 1});
	if (y > 0 && map[y - 1][x] == FLOOR)
		return ((t_point){x, y - 1});
	if (map[y][x + 1] == FLOOR)
		return ((t_point){x + 1, y});
	if (x > 0 && map[y][x - 1] == FLOOR)
		return ((t_point){x - 1, y});
	return ((t_point){-1, -1});
}

bool	get_doors(int **map, t_door_list **doors)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != '\0')
		{
			if (!fetch_door(map, doors, x, y))
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}

static bool	fetch_door(int **map, t_door_list **doors,
			size_t x, size_t y)
{
	t_door_list	*door;

	if (map[y][x] == DOOR)
	{
		door = append_door_node(doors, create_door_node((t_point){x, y}));
		if (!door)
			return (false);
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
