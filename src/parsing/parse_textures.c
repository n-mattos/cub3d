/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:46 by nmattos           #+#    #+#             */
/*   Updated: 2025/06/24 13:03:40 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	skip_whitespaces(char *str, int i)
{
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	return (i);
}

size_t	count_char(char *str, char c)
{
	int		i;
	size_t	n;

	i = 0;
	n = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			n++;
		i++;
	}
	return (n);
}

static size_t	chars_till_eol(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	if (i == 0)
		return (0);
	return (i + 1);
}

char	*get_texture(char *str)
{
	char	*texture;
	size_t	i;
	size_t	j;

	texture = malloc(sizeof(char) *
					(chars_till_eol(str) - count_char(str, ' ') + 1));
	if (texture == NULL)
		return (NULL);
	j = 0;
	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		i = skip_whitespaces(str, i);
		texture[j++] = str[i++];
	}
	texture[j] = '\0';
	return (texture);
}

char	**get_raw_texture_data(int fd)
{
	char	**raw_textures;
	size_t	n_data;		// Number of data stored. (6 = NO/EA/SO/WE / F/C)
	char	*line;
	size_t	i;

	n_data = 0;
	i = 0;
	line = get_next_line(fd);
	raw_textures = malloc(sizeof(char *) * 6);
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
	return (raw_textures);
}

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

void	sort_texture_data(char **raw_textures, t_textures **textures)
{
	size_t	i;

	i = -1;
	while (++i < 6)
	{
		if (raw_textures[i][0] == 'N')
			(*textures)->north = raw_textures[i];
		else if (raw_textures[i][0] == 'E')
			(*textures)->east = raw_textures[i];
		else if (raw_textures[i][0] == 'S')
			(*textures)->south = raw_textures[i];
		else if (raw_textures[i][0] == 'W')
			(*textures)->west = raw_textures[i];
		else if (raw_textures[i][0] == 'F')
			(*textures)->floor = raw_textures[i];
		else if (raw_textures[i][0] == 'C')
			(*textures)->ceiling = raw_textures[i];
		else
		{
			free_raw_textures(raw_textures);
			free(*textures);
			*textures = NULL;
			return ;
		}
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
	if (raw_textures == NULL)
	{
		free(textures);
		return (NULL);
	}
	sort_texture_data(raw_textures, &textures);
	return (textures);
}
