/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:57:43 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/24 15:11:08 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	invalid_tile(int **map, size_t y, size_t x)
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

bool	map_is_valid(int **map)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != '\0')
		{
			if (invalid_tile(map, y, x))
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}
