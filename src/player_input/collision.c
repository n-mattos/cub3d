/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:01:23 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 16:04:53 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	handle_portal(t_level *level, t_playerdata *p, int y, int x);
static int	get_direction(t_point A, t_point B);

/**
 * Handles player collision with walls.
 * @param d Pointer to the game data structure.
 * @param p Pointer to the player data structure.
 * @param new_x The new x-coordinate of the player.
 * @param new_y The new y-coordinate of the player.
 * @returns void; Updates the player's position if the new coordinates
 * 				  do not collide with walls.
 */
void	collision(t_level *level, t_vect new)
{
	t_playerdata	*p;

	p = level->player;
	if (new.y > p->y)
	{
		if (handle_portal(level, p, (int)(new.y + COLLISION_BUFFER), (int)p->x))
			return ;
		if (level->map[(int)(new.y + COLLISION_BUFFER)][(int)p->x] != WALL
			&& level->map[(int)(new.y + COLLISION_BUFFER)][(int)p->x] != DOOR)
			p->y = new.y;
	}
	else
	{
		if (handle_portal(level, p, (int)(new.y - COLLISION_BUFFER), (int)p->x))
			return ;
		if (level->map[(int)(new.y - COLLISION_BUFFER)][(int)p->x] != WALL
			&& level->map[(int)(new.y - COLLISION_BUFFER)][(int)p->x] != DOOR)
			p->y = new.y;
	}
	if (new.x < p->x)
	{
		if (handle_portal(level, p, (int)p->y, (int)(new.x - COLLISION_BUFFER)))
			return ;
		if (level->map[(int)p->y][(int)(new.x - COLLISION_BUFFER)] != WALL
			&& level->map[(int)(new.x - COLLISION_BUFFER)][(int)p->x] != DOOR)
			p->x = new.x;
	}
	else
	{
		if (handle_portal(level, p, (int)p->y, (int)(new.x + COLLISION_BUFFER)))
			return ;
		if (level->map[(int)p->y][(int)(new.x + COLLISION_BUFFER)] != WALL
			&& level->map[(int)(new.x + COLLISION_BUFFER)][(int)p->x] != DOOR)
			p->x = new.x;
	}
}

/**
 * Handles player teleportation through portals.
 * @param level Pointer to the level data structure.
 * @param p Pointer to the player data structure.
 * @param y The y-coordinate of the portal tile.
 * @param x The x-coordinate of the portal tile.
 * @returns int; 1 if the player was teleported, 0 otherwise.
 */
static int	handle_portal(t_level *level, t_playerdata *p, int y, int x)
{
	t_portal_list	*portal;

	portal = find_portal_node(level->portals, level->map[y][x]);
	if (portal)
	{
		if (portal->A[SOURCE].x == x && portal->A[SOURCE].y == y)
		{
			player_starting_direction(p,
				get_direction(portal->B[SOURCE], portal->A[TARGET]));
			p->x = portal->A[TARGET].x + 0.5;
			p->y = portal->A[TARGET].y + 0.5;
			return (1);
		}
		else
		{
			player_starting_direction(p,
				get_direction(portal->A[SOURCE], portal->B[TARGET]));
			p->x = portal->B[TARGET].x + 0.5;
			p->y = portal->B[TARGET].y + 0.5;
			return (1);
		}
	}
	return (0);
}

/**
 * Determines the cardinal direction from point A to point B.
 * @param A The starting point.
 * @param B The ending point.
 * @returns int; The direction from A to B (NORTH, SOUTH, EAST, WEST).
 */
static int	get_direction(t_point A, t_point B)
{
	if (A.x < B.x && A.y == B.y)
		return (EAST);
	if (A.x > B.x && A.y == B.y)
		return (WEST);
	if (A.y < B.y && A.x == B.x)
		return (SOUTH);
	if (A.y > B.y && A.x == B.x)
		return (NORTH);
	return (-1);
}
