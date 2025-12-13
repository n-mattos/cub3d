/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_allocate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 16:39:31 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/13 16:39:35 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Allocates memory for a t_textures structure and its string members.
 * @return Pointer to the allocated t_textures structure, or NULL on failure.
 */
t_textures	*allocate_textures(void)
{
	t_textures	*textures;
	int			i;

	textures = malloc(sizeof(t_textures));
	if (textures == NULL)
		return (NULL);
	textures->north = NULL;
	textures->east = NULL;
	textures->south = NULL;
	textures->west = NULL;
	textures->tex_line_offset = 0;
	textures->floor = -5;
	textures->ceiling = -5;
	textures->redef_check = (t_tex_redef_check){0, 0, 0, 0, 0, 0};
	i = 0;
	while (i < 6)
		textures->portal[i++] = NULL;
	i = 0;
	while (i < 21)
		textures->door[i++] = NULL;
	return (textures);
}
