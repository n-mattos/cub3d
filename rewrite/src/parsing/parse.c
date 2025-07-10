/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:52 by nmattos           #+#    #+#             */
/*   Updated: 2025/06/26 13:31:46 by nmattos-         ###   ########.fr       */
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
	{
		free_textures(textures);
		free_map(level->map, -1);
		perror("Map is invalid\n");
		return (NULL);
	}
	level->player = retrieve_player(level->map);
	if (level->player == NULL)
		return (perror("Player not found in map\n"), free_level(level), NULL);
	return (level);
}
