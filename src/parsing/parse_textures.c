/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:46 by nmattos           #+#    #+#             */
/*   Updated: 2025/08/26 12:29:55 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static char	**get_raw_texture_data(int fd);
static char	*get_texture(char *str);
static t_textures	*sort_texture_data(char **raw, t_textures *textures);

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
 * Reads texture data from a file descriptor and returns it as an array of strings.
 * @param fd The file descriptor to read from.
 * @return An array of strings containing the raw texture data,
 * 		   or NULL on error.
 */
static char	**get_raw_texture_data(int fd)
{
	char	**raw_textures;
	size_t	n_data;
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
				return (free(line), NULL);
			n_data++;
		}
		free(line);
		line = get_next_line(fd);
	}
	free(line);
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

bool	transform_rgb_string(char *rgb)
{
	int	index;
	int	commacount;

	index = 0;
	commacount = 0;
	while (rgb[index])
	{
		if (rgb[index] == ',')
		{
			commacount++;
			if (commacount > 2)
				return (false);
			if (!ft_isdigit(rgb[index + 1]))
				return (false);
			rgb[index] = '\0';
		}
		else if (!ft_isdigit(rgb[index]))
			return (false);
		index++;
	}
	return (true);
}

int	rgbfromstr(char *rgb)
{
	int	r;
	int	g;
	int	b;

	if (!transform_rgb_string(rgb))
		return (-1);
	r = ft_atoi(rgb);
	while (*rgb)
		rgb++;
	rgb++;
	g = ft_atoi(rgb);
	while (*rgb)
		rgb++;
	rgb++;
	b = ft_atoi(rgb);
	return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

/**
 * Sorts the raw texture data into the t_textures structure.
 * @param raw The array of raw texture strings.
 * @param textures The t_textures structure to fill.
 * @return A pointer to the filled t_textures structure,
 *         or NULL if an error occurs.
 */
static t_textures	*sort_texture_data(char **raw, t_textures *textures)
{
	size_t	i;

	i = -1;
	while (++i < 6)
	{
		if (ft_strncmp(raw[i], "NO", 2) == 0)
			textures->north = mlx_load_png(raw[i] + 2);
		else if (ft_strncmp(raw[i], "EA", 2) == 0)
			textures->east = mlx_load_png(raw[i] + 2);
		else if (ft_strncmp(raw[i], "SO", 2) == 0)
			textures->south = mlx_load_png(raw[i] + 2);
		else if (ft_strncmp(raw[i], "WE", 2) == 0)
			textures->west = mlx_load_png(raw[i] + 2);
		else if (raw[i][0] == 'F')
		{
			textures->floor = rgbfromstr(raw[i] + 1);
			if (textures->floor == -1)
				return (free_raw_textures(raw), free_textures(textures), NULL);
		}
		else if (raw[i][0] == 'C')
		{
			textures->ceiling = rgbfromstr(raw[i] + 1);
			if (textures->ceiling == -1)
				return (free_raw_textures(raw), free_textures(textures), NULL);
		}
		else
			return (free_raw_textures(raw), free_textures(textures), NULL);
	}
	return (free_raw_textures(raw), textures);
}
