/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:19:54 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/26 14:04:59 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_playerdata	*create_player(int **map, size_t y, size_t x);
static int	player_starting_direction(int direction);

t_playerdata	*retrieve_player(int **map)
{
	size_t	x;
	size_t	y;

	if (map == NULL)
		return (NULL);
	y = 0;
	while (map[y] != NULL)
	{
		x = 0;
		while (map[y][x] != '\0')
		{
			if (map[y][x] == NORTH ||
				map[y][x] == EAST  ||
				map[y][x] == SOUTH ||
				map[y][x] == WEST)
			{
				return (create_player(map, y, x));
			}
			x++;
		}
		y++;
	}
	return (NULL);
}

static t_playerdata	*create_player(int **map, size_t y, size_t x)
{
	t_playerdata	*player;

	player = (t_playerdata *)malloc(sizeof(t_playerdata));
	if (player == NULL)
		return (NULL);
	player->x = x;
	player->y = y;
	player->degrees = player_starting_direction(map[y][x]);
	if (player->degrees == -1)
	{
		free(player);
		return (NULL);
	}
	return (player);
}

static int	player_starting_direction(int direction)
{
	if (direction == NORTH)
		return (270);
	else if (direction == EAST)
		return (0);
	else if (direction == SOUTH)
		return (90);
	else if (direction == WEST)
		return (180);
	return (-1);
}
