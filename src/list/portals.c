/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:06:03 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 15:02:46 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cub3d.h"

/**
 * Creates a new portal list node with the given id,
 * source point A, and target point for B.
 * @param id Character identifier for the portal.
 * @param sourceA t_point representing the source point of the A portal.
 * @param targetB t_point representing the target point of the B portal.
 * @return Pointer to the newly created node, or NULL if memory allocation fails.
 */
t_portal_list	*create_portal_node(char id, t_point sourceA, t_point targetB)
{
	t_portal_list	*new_node;

	new_node = malloc(sizeof(t_portal_list));
	if (!new_node)
		return (NULL);
	new_node->id = id;
	new_node->A[SOURCE] = sourceA;
	new_node->A[TARGET] = INVALID_POINT;
	new_node->B[SOURCE] = INVALID_POINT;
	new_node->B[TARGET] = targetB;
	new_node->next = NULL;
	return (new_node);
}

/**
 * Appends a new node to the end of the portal list.
 * @param head Double pointer to the head of the portal list.
 * @param new_node Pointer to the new node to be appended.
 * @return Pointer to the newly appended node, or NULL if new_node is NULL.
 */
t_portal_list	*append_portal_node(t_portal_list **head,
		t_portal_list *new_node)
{
	t_portal_list	*current;

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

/**
 * Finds a node in the portal list by its id.
 * @param head Pointer to the head of the portal list.
 * @param id Character identifier of the portal to find.
 * @return Pointer to the found node, or NULL if not found.
 */
t_portal_list	*find_portal_node(t_portal_list *head, char id)
{
	t_portal_list	*current;

	current = head;
	while (current != NULL)
	{
		if (current->id == id)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/**
 * Updates the source B and target A points of a given portal list node.
 * @param node Pointer to the portal list node to be updated.
 * @param sourceB t_point representing the new source point of the B portal.
 * @param targetA t_point representing the new target point of the A portal.
 */
void	update_portal_node(t_portal_list *node, t_point sourceB,
		t_point targetA)
{
	node->B[SOURCE] = sourceB;
	node->A[TARGET] = targetA;
}

/**
 * Frees the entire portal list and its nodes.
 * @param head Pointer to the head of the portal list.
 * @return NULL after freeing the list.
 */
t_portal_list	*free_portal_list(t_portal_list **head)
{
	t_portal_list	*current;
	t_portal_list	*next;

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
