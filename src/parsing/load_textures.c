/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:25:18 by nmattos           #+#    #+#             */
/*   Updated: 2025/12/12 14:43:06 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_textures	*load_textures(char **raw, t_textures *textures);
static bool			check_texture_validity(t_textures *textures);
static int			rgbfromstr(char *rgb);
static bool			transform_rgb_string(char *rgb);

/**
 * Sorts the raw texture data into the t_textures structure.
 * @param raw The array of raw texture strings.
 * @param textures The t_textures structure to fill.
 * @return A pointer to the filled t_textures structure,
 *         or NULL if an error occurs.
 */
t_textures	*sort_texture_data(char **raw, t_textures *textures)
{
	textures = load_textures(raw, textures);
	if (textures == NULL)
		return (NULL);
	textures->portal[0] = mlx_load_png("textures/portal/1.png");
	textures->portal[1] = mlx_load_png("textures/portal/2.png");
	textures->portal[2] = mlx_load_png("textures/portal/3.png");
	textures->portal[3] = mlx_load_png("textures/portal/4.png");
	textures->portal[4] = mlx_load_png("textures/portal/5.png");
	textures->portal[5] = mlx_load_png("textures/portal/6.png");
	textures = load_door_textures(textures);
	if (check_texture_validity(textures) == false)
		return (free_raw_textures(raw), free_textures(textures), NULL);
	return (free_raw_textures(raw), textures);
}

static t_textures	*load_door_textures(t_textures *textures)
{
	textures->door[0] = mlx_load_png("textures/door/1.png");
	textures->door[1] = mlx_load_png("textures/door/2.png");
	textures->door[2] = mlx_load_png("textures/door/3.png");
	textures->door[3] = mlx_load_png("textures/door/4.png");
	textures->door[4] = mlx_load_png("textures/door/5.png");
	textures->door[5] = mlx_load_png("textures/door/6.png");
	textures->door[6] = mlx_load_png("textures/door/7.png");
	textures->door[7] = mlx_load_png("textures/door/8.png");
	textures->door[8] = mlx_load_png("textures/door/9.png");
	textures->door[9] = mlx_load_png("textures/door/10.png");
	textures->door[10] = mlx_load_png("textures/door/11.png");
	textures->door[11] = mlx_load_png("textures/door/12.png");
	textures->door[12] = mlx_load_png("textures/door/13.png");
	textures->door[13] = mlx_load_png("textures/door/14.png");
	textures->door[14] = mlx_load_png("textures/door/15.png");
	textures->door[15] = mlx_load_png("textures/door/16.png");
	textures->door[16] = mlx_load_png("textures/door/17.png");
	textures->door[17] = mlx_load_png("textures/door/18.png");
	textures->door[18] = mlx_load_png("textures/door/19.png");
	textures->door[19] = mlx_load_png("textures/door/20.png");
	textures->door[20] = mlx_load_png("textures/door/21.png");
	return (textures);
}

/**
 * Loads textures from the raw data into the t_textures structure.
 * @param raw The array of raw texture strings.
 * @param textures The t_textures structure to fill.
 * @return A pointer to the filled t_textures structure,
 *         or NULL if an error occurs.
 */
static t_textures	*load_textures(char **raw, t_textures *textures)
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
			textures->floor = rgbfromstr(raw[i] + 1);
		else if (raw[i][0] == 'C')
			textures->ceiling = rgbfromstr(raw[i] + 1);
		else
			return (free_raw_textures(raw), free_textures(textures), NULL);
	}
	return (textures);
}

/**
 * Checks if all required textures are loaded and valid.
 * @param textures The t_textures structure to check.
 * @return true if all textures are valid, false otherwise.
 */
static bool	check_texture_validity(t_textures *textures)
{
	if (textures->north == NULL || textures->east == NULL
		|| textures->south == NULL || textures->west == NULL
		|| textures->floor == -1 || textures->ceiling == -1
		|| textures->portal[0] == NULL || textures->portal[1] == NULL
		|| textures->portal[2] == NULL || textures->portal[3] == NULL
		|| textures->portal[4] == NULL || textures->portal[5] == NULL)
	{
		return (false);
	}
	return (true);
}

/**
 * Converts an RGB string to an integer representation.
 * @param rgb The RGB string in the format "R,G,B".
 * @return The integer representation of the RGB color,
 *         or -1 if the string is invalid.
 */
static int	rgbfromstr(char *rgb)
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
 * Transforms an RGB string by replacing commas with null terminators.
 * @param rgb The RGB string to transform.
 * @return true if the transformation is successful, false otherwise.
 */
static bool	transform_rgb_string(char *rgb)
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
