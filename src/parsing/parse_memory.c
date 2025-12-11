/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:39:41 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/11 16:10:53 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Frees a 2D array of strings (raw textures).
 * @param raw_textures Pointer to the 2D array to be freed.
 */
void	free_raw_textures(char **raw_textures)
{
	size_t	i;

	if (raw_textures == NULL)
		return ;
	i = 0;
	while (i < 6)
	{
		if (raw_textures[i] != NULL)
			free(raw_textures[i]);
		i++;
	}
	free(raw_textures);
}

/**
 * Frees the memory allocated for a t_textures structure and its string members.
 * @param textures Pointer to the t_textures structure to be freed.
 */
void	free_textures(t_textures *textures)
{
	int	i;

	if (textures == NULL)
		return ;
	if (textures->north != NULL)
		mlx_delete_texture(textures->north);
	if (textures->east != NULL)
		mlx_delete_texture(textures->east);
	if (textures->south != NULL)
		mlx_delete_texture(textures->south);
	if (textures->west != NULL)
		mlx_delete_texture(textures->west);
	i = 0;
	while (i < 6)
	{
		if (textures->portal[i] != NULL)
			mlx_delete_texture(textures->portal[i]);
		i++;
	}
	i = 0;
	while (i < 21)
	{
		if (textures->door[i] != NULL)
			mlx_delete_texture(textures->door[i]);
		i++;
	}
	free(textures);
}

/**
 * Frees a 2D array of integers (map) and its rows.
 * @param map Pointer to the 2D array to be freed.
 * @param i Index to start freeing from; if negative, frees until NULL is found.
 */
void	free_map(int **map, int i)
{
	if (i < 0)
	{
		i = 0;
		while (map[i] != NULL)
			free(map[i++]);
	}
	else
	{
		while (i >= 0)
			free(map[i--]);
	}
	free(map);
}

/**
 * Frees the memory allocated for a t_level structure and its components.
 * @param level Pointer to the t_level structure to be freed.
 */
void	free_level(t_level *level)
{
	free_textures(level->textures);
	free_map(level->map, -1);
	if (level->player != NULL)
		free(level->player);
	free(level);
}

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
	return (textures);
}
