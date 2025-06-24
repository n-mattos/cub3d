/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_textures.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos <nmattos@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:10:46 by nmattos       #+#    #+#                 */
/*   Updated: 2025/06/24 09:46:26 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	*get_texture(char *str)
{
	char	*texture;
	size_t	i;
	size_t	j;

	while (str[i] != '\0' && str[i] != '\n')
	{
		i = skip_whitespaces(str, i);
		texture[j++] = str[i++]
	}
	texture[j] = '\0';
	return (texture);
}

char	**get_raw_texture_data(int fd)
{
	char	**raw_textures;
	size_t	n_data;		// Number of data stored. (6 = NO/EA/SO/WE / F/C)
	char	*line
	size_t	i;

	n_data = 0;
	i = 0;
	line = get_next_line(fd);
	raw_textures = malloc(sizeof(char *) * (6 + 1));
	if (raw_textures == NULL)
		return (NULL);
	while (line && n_data < 6)
	{
		i = skip_whitespaces(line, 0);
		if (line[i] != '\n' && line[i] != '\0')
		{
			raw_textures[n_data] = get_texture(line);
			if (raw_textures[n_data] == NULL)
				return (NULL);
			n_data++;
		}
		free(line);
		line = get_next_line(fd);
	}
}

t_textures	*parse_textures(int fd)
{
	t_textures	*textures;
	char		**raw_textures;

	textures = malloc(sizeof(t_textures));
	if (textures == NULL)
		return (NULL);
	raw_textures = get_raw_texture_data(fd);
}
