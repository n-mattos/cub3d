/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:52 by nmattos           #+#    #+#             */
/*   Updated: 2025/09/25 12:27:12 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Parses a .cub file to create a t_level structure.
 * @param fn_map The path to the .cub file.
 * @return A pointer to the t_level structure if successful, or NULL on failure.
 */
t_level	*parse(char *fn_map)
{
	t_level		*level;
	t_textures	*textures;
	int			fd;

	fd = open(fn_map, O_RDONLY);
	if (fd == -1)
		return (perror("Error\nFailed to open .cub file"), NULL);
	textures = parse_textures(fd);
	if (textures == NULL)
		return (perror("Error\ntextures == NULL"), NULL);
	level = parse_map(fd);
	if (level == NULL)
		return (perror("Error\nmap == NULL"), free_textures(textures), NULL);
	level->textures = textures;
	if (!map_is_valid(level->map))
	{
		free_textures(textures);
		free_map(level->map, -1);
		perror("Error\nMap is invalid\n");
		return (NULL);
	}
	if (!get_portals(level->map, &level->portals))
		return (perror("Error\nInvalid portals\n"), free_level(level), NULL);
	level->player = retrieve_player(level->map);
	if (level->player == NULL)
		return (perror("Error\nPlayer not found in map"), free_level(level), NULL);
	return (level);
}
