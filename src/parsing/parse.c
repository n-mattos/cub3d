/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:52 by nmattos           #+#    #+#             */
/*   Updated: 2025/06/24 11:40:41 by nmattos-         ###   ########.fr       */
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
		return (NULL);
	textures = parse_textures(fd);
	if (textures == NULL)
		return (NULL);
	level = parse_map(fd);
	if (level == NULL)
		return (NULL);
	level->textures = textures;
	return (level);
}
