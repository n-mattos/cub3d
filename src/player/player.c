/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:19:54 by nmattos-          #+#    #+#             */
/*   Updated: 2025/07/10 13:30:53 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_playerdata	*create_player(int **map, size_t y, size_t x);
static int	player_starting_direction(t_playerdata *player, int direction);

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
	player->x = x * 1000000;
	player->y = y * 1000000;
	if (player_starting_direction(player, map[y][x]) == -1)
	{
		free(player);
		return (NULL);
	}
	player->plane_x = 0;
	player->plane_y = 0.66;
	return (player);
}

static int	player_starting_direction(t_playerdata *player, int direction)
{
	if (direction == NORTH)
	{
		player->dir_x = -1;
		player->dir_y = 0;
	}
	else if (direction == EAST)
	{
		player->dir_x = 0;
		player->dir_y = -1;
	}
	else if (direction == SOUTH)
	{
		player->dir_x = 1;
		player->dir_y = 0;
	}
	else if (direction == WEST)
	{
		player->dir_x = 0;
		player->dir_y = 1;
	}
	else
		return (-1);
	return (1);
}
