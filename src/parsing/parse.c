/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:52 by nmattos           #+#    #+#             */
/*   Updated: 2025/06/25 10:22:24 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_level	*parse(char *fn_map)
{
	t_level		*level;
	t_textures	*textures;
	int			fd;

	fd = open(fn_map, O_RDONLY);
	if (fd == -1)
		return (perror("Failed to open .cub file"), NULL);
	textures = parse_textures(fd);
	if (textures == NULL)
		return (perror("textures == NULL\n"), NULL);
	level = parse_map(fd);
	if (level == NULL)
		return (perror("map == NULL\n"), free_textures(textures), NULL);
	level->textures = textures;
	if (!map_is_valid(level->map))
		return (perror("Map is invalid\n"), free_level(level), NULL);
	return (level);
}
