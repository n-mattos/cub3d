/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_doors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:42:13 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/13 16:42:58 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool		fetch_door(int **map, t_door_list **doors,
					size_t x, size_t y);

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
