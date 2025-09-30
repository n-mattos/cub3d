/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_textures.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos- <nmattos-@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:10:46 by nmattos       #+#    #+#                 */
/*   Updated: 2025/09/30 09:38:11 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static char			**get_raw_texture_data(int fd);
static char			**get_raw_texture(int fd, char *line, char **raw_textures);
static char			*get_texture(char *str);

/**
 * Parses textures from a file descriptor.
 * @param fd The file descriptor to read from.
 * @return A pointer to a t_textures structure containing the parsed textures,
 *         or NULL if an error occurs.
 */
t_textures	*parse_textures(int fd)
{
	t_textures	*textures;
	char		**raw_textures;

	textures = allocate_textures();
	if (textures == NULL)
		return (NULL);
	raw_textures = get_raw_texture_data(fd);
	if (raw_textures == NULL)
	{
		free(textures);
		return (NULL);
	}
	textures = sort_texture_data(raw_textures, textures);
	return (textures);
}

/**
 * Reads texture data from a file descriptor and
 * returns it as an array of strings.
 * @param fd The file descriptor to read from.
 * @return An array of strings containing the raw texture data,
 * 		   or NULL on error.
 */
static char	**get_raw_texture_data(int fd)
{
	char	**raw_textures;
	char	*line;

	line = get_next_line(fd);
	if (line == NULL)
		return (perror("Error\nFile is empty"), NULL);
	raw_textures = malloc(sizeof(char *) * 6);
	if (raw_textures == NULL)
		return (perror("Error\nMalloc for textures failed"), NULL);
	raw_textures = get_raw_texture(fd, line, raw_textures);
	if (raw_textures == NULL)
		return (perror("Error\nMalloc for textures failed"), NULL);
	free(line);
	return (raw_textures);
}

static char	**get_raw_texture(int fd, char *line, char **raw_textures)
{
	size_t	n_data;
	size_t	i;

	n_data = 0;
	while (line && n_data < 7)
	{
		i = skip_whitespaces(line, 0);
		if (line[i] != '\n' && line[i] != '\0')
		{
			raw_textures[n_data] = get_texture(line);
			if (raw_textures[n_data] == NULL)
				return (free(line), free_raw_textures(raw_textures), NULL);
			n_data++;
		}
		free(line);
		line = get_next_line(fd);
	}
	return (raw_textures);
}

/**
 * Extracts a texture string from a line, skipping leading whitespace.
 * @param str The string to extract the texture from.
 * @return A dynamically allocated string containing the texture,
 * 		   or NULL on error.
 */
static char	*get_texture(char *str)
{
	char	*texture;
	size_t	i;
	size_t	j;

	texture = malloc(sizeof(char)
			* (chars_till_eol(str) - count_char(str, ' ') + 1));
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
