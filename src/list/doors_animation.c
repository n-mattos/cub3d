/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_animation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:26:56 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/03 14:20:04 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	next_frame(t_door_list *door, t_level *level);

mlx_texture_t	*get_door_texture(t_door_list *door, t_point door_pos)
{
	t_door_list	*node;

	node = find_door_node(door, door_pos);
	if (!node || !node->texture)
		return (NULL);
	return (node->texture);
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

void	update_doors(t_door_list *doors, t_level *level)
{
	t_door_list	*current;

	current = doors;
	while (current)
	{
		if (current->state == CLOSED || current->state == OPEN)
		{
			current->texture = level->textures->door[current->index];
			current = current->next;
			continue ;
		}
		next_frame(current, level);
		current->texture = level->textures->door[current->index];
		current = current->next;
	}
}

static void	next_frame(t_door_list *door, t_level *level)
{
	if (door->state == OPENING)
		door->index += 1;
	else if (door->state == CLOSING)
	{
		door->index -= 1;
		level->map[door->pos.y][door->pos.x] = DOOR;
	}
	if (door->index <= 0)
	{
		door->index = 0;
		door->state = CLOSED;
		level->map[door->pos.y][door->pos.x] = DOOR;
	}
	else if (door->index >= 20)
	{
		door->index = 20;
		door->state = OPEN;
		level->map[door->pos.y][door->pos.x] = DOOR_OPEN;
	}
}
