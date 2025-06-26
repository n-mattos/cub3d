/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 09:39:41 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/26 13:27:43 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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

void	free_textures(t_textures *textures)
{
	if (textures == NULL)
		return ;
	if (textures->north != NULL)
		free(textures->north);
	if (textures->east != NULL)
		free(textures->east);
	if (textures->south != NULL)
		free(textures->south);
	if (textures->west != NULL)
		free(textures->west);
	if (textures->floor != NULL)
		free(textures->floor);
	if (textures->ceiling != NULL)
		free(textures->ceiling);
	free(textures);
}

void	free_map(int **map, size_t i)
{
	if (i < 0)
	{
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

void	free_level(t_level *level)
{
	free_textures(level->textures);
	free_map(level->map, -1);
	if (level->player != NULL)
		free(level->player);
	free(level);
}

t_textures	*allocate_textures(void)
{
	t_textures	*textures;

	textures = malloc(sizeof(t_textures));
	if (textures == NULL)
		return (NULL);
	textures->north = malloc(sizeof(char) * 256);
	textures->east = malloc(sizeof(char) * 256);
	textures->south = malloc(sizeof(char) * 256);
	textures->west = malloc(sizeof(char) * 256);
	textures->floor = malloc(sizeof(char) * 256);
	textures->ceiling = malloc(sizeof(char) * 256);
	if (textures->north == NULL ||
		textures->east == NULL  ||
		textures->south == NULL ||
		textures->west == NULL  ||
		textures->floor == NULL ||
		textures->ceiling == NULL)
	{
		free_textures(textures);
		return (NULL);
	}
	return (textures);
}
