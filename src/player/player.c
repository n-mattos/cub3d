/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:19:54 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 16:15:19 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_playerdata	*create_player(int **map, size_t y, size_t x);
static void			set_player_direction(
						t_playerdata *player, t_point direction, t_vect plane);

/**
 * Retrieves the player data from the map.
 * @param map The game map represented as a 2D array of integers.
 * @return A pointer to the player data structure,
 *         or NULL if the player is not found or memory allocation fails.
 */
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
			if (map[y][x] == NORTH
				|| map[y][x] == EAST
				|| map[y][x] == SOUTH
				|| map[y][x] == WEST)
			{
				return (create_player(map, y, x));
			}
			x++;
		}
		y++;
	}
	return (NULL);
}

/**
 * Creates a player data structure based on the map position.
 * @param map The game map represented as a 2D array of integers.
 * @param y The y-coordinate of the player's position in the map.
 * @param x The x-coordinate of the player's position in the map.
 * @return A pointer to the newly created player data structure,
 *         or NULL if memory allocation fails or the direction is invalid.
 */
static t_playerdata	*create_player(int **map, size_t y, size_t x)
{
	t_playerdata	*player;

	player = (t_playerdata *)malloc(sizeof(t_playerdata));
	if (player == NULL)
		return (NULL);
	player->x = x + 0.5;
	player->y = y + 0.5;
	if (player_starting_direction(player, map[y][x]) == -1)
	{
		free(player);
		return (NULL);
	}
	return (player);
}

/**
 * Sets the player's starting direction based on the map value.
 * @param player Pointer to the player data structure to be updated.
 * @param direction The direction represented by an integer
 * 					(NORTH, EAST, SOUTH, WEST).
 * @return 1 if successful, -1 if the direction is invalid.
 */
int	player_starting_direction(t_playerdata *player, int direction)
{
	if (direction == NORTH)
		set_player_direction(player, (t_point){0, -1}, (t_vect){0.66, 0});
	else if (direction == EAST)
		set_player_direction(player, (t_point){1, 0}, (t_vect){0, 0.66});
	else if (direction == SOUTH)
		set_player_direction(player, (t_point){0, 1}, (t_vect){-0.66, 0});
	else if (direction == WEST)
		set_player_direction(player, (t_point){-1, 0}, (t_vect){0, -0.66});
	else
		return (-1);
	return (1);
}

/**
 * Sets the player's direction and camera plane.
 * @param player Pointer to the player data structure to be updated.
 * @param direction The direction vector representing the player's facing direction.
 * @param plane The camera plane vector perpendicular to the direction.
 */
static void	set_player_direction(
				t_playerdata *player, t_point direction, t_vect plane)
{
	player->dir_x = direction.x;
	player->dir_y = direction.y;
	player->plane_x = plane.x;
	player->plane_y = plane.y;
}
