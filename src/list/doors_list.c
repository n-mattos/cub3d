/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 10:21:24 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/03 11:10:17 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_door_list	*create_door_node(t_point position)
{
	t_door_list	*new_node;

	new_node = malloc(sizeof(t_door_list));
	if (!new_node)
		return (NULL);
	new_node->pos = position;
	new_node->state = CLOSED;
	new_node->index = 0;
	new_node->texture = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_door_list	*append_door_node(t_door_list **head, t_door_list *new_node)
{
	t_door_list	*current;

	if (!new_node)
		return (NULL);
	if (*head == NULL)
	{
		*head = new_node;
		return (new_node);
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	return (new_node);
}

t_door_list	*find_door_node(t_door_list *head, t_point pos)
{
	t_door_list	*current;

	current = head;
	while (current != NULL)
	{
		if (current->pos.x == pos.x && current->pos.y == pos.y)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_door_list	*free_door_list(t_door_list **head)
{
	t_door_list	*current;
	t_door_list	*next;

	current = *head;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
	return (NULL);
}
