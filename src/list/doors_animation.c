/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_animation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:26:56 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/03 12:19:23 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

mlx_texture_t	*get_door_texture(t_door_list *door, t_point door_pos)
{
	t_door_list	*node;

	node = find_door_node(door, door_pos);
	if (!node || !node->texture)
		return (NULL);
	return (node->texture);
}

void	update_doors(t_door_list *doors, t_level *level, double delta_time)
{
	t_door_list	*current;
	(void)delta_time;

	current = doors;
	while (current)
	{
		if (current->state == CLOSED || current->state == OPEN)
		{
			current->texture = level->textures->door[current->index];
			current = current->next;
			continue ;
		}
		if (current->state == OPENING)
			current->index += 1;
		else if (current->state == CLOSING)
		{
			current->index -= 1;
			level->map[current->pos.y][current->pos.x] = DOOR;
		}
		if (current->index <= 0)
		{
			current->index = 0;
			current->state = CLOSED;
			level->map[current->pos.y][current->pos.x] = DOOR;
		}
		else if (current->index >= 20)
		{
			current->index = 20;
			current->state = OPEN;
			level->map[current->pos.y][current->pos.x] = DOOR_OPEN;
		}
		current->texture = level->textures->door[current->index];
		current = current->next;
	}
}

void	trigger_door(t_level *level, t_point door_pos)
{
	t_door_list	*door;

	door = find_door_node(level->doors, door_pos);
	if (!door)
		return ;
	if (door->state == CLOSED)
		door->state = OPENING;
	else if (door->state == OPEN)
		door->state = CLOSING;
}
