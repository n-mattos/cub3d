/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_parser3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:16:38 by mschippe          #+#    #+#             */
/*   Updated: 2025/12/13 17:18:33 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_tex_info_type	get_tex_type(char *line)
{
	line += num_spaces(line);
	if (!ft_strncmp(line, "NO ", 3))
		return (TI_NORTH);
	if (!ft_strncmp(line, "EA ", 3))
		return (TI_EAST);
	if (!ft_strncmp(line, "SO ", 3))
		return (TI_SOUTH);
	if (!ft_strncmp(line, "WE ", 3))
		return (TI_WEST);
	if (!ft_strncmp(line, "F ", 2))
		return (TI_FLOOR);
	if (!ft_strncmp(line, "C ", 2))
		return (TI_CEILING);
	return (TI_NONE);
}

char	*get_tex_value(char *line)
{
	char	*start;
	int		count;

	count = 0;
	line += num_spaces(line);
	while (!is_whitespace(*line++))
		continue ;
	line += num_spaces(line);
	start = line;
	while (line && *line && !is_whitespace(*line++))
	{
		if (is_whitespace(*line))
		{
			if (!*(line + num_spaces(line)) && num_spaces(line) > 0)
				*line = '\0';
			count += num_spaces(line);
			line += num_spaces(line);
		}
	}
	return (ft_strdup(start));
}

bool	rgb_atoi(const char *nptr, int *target)
{
	long	sig;
	long	res;
	bool	success;

	res = 0;
	sig = 1;
	success = false;
	while ((*nptr > 8 && *nptr < 14) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
		sig = (*nptr++ - 44) * -1;
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + (*nptr++ - '0');
		if (res > 255 || res < 0)
			return (false);
		success = true;
	}
	while ((*nptr > 8 && *nptr < 14) || *nptr == 32)
		nptr++;
	if (*nptr || !success)
		return (false);
	*target = (int)(sig * res);
	return (true);
}

/**
 * Counts and returns the amount of elements in a 2D array of any type
 * Expects the array to be NULL terminated and does not count the NULL
 * TODO: Add to libft
 */
size_t	ft_arrlen(void **arr)
{
	size_t	size;

	size = 0;
	if (!arr)
		return (size);
	while (*arr++)
		size++;
	return (size);
}

int	string_to_rgb(char *value)
{
	char	**split;
	int		red;
	int		green;
	int		blue;

	red = 0;
	green = 0;
	blue = 0;
	split = ft_split(value, ',');
	if (!split)
		return (-2);
	if (ft_arrlen((void **)split) != 3)
		return (free_2d_array(split), -3);
	if (!rgb_atoi(split[0], &red)
		|| !rgb_atoi(split[1], &green)
		|| !rgb_atoi(split[2], &blue))
		return (free_2d_array(split), -4);
	free_2d_array(split);
	return ((red << 24) | (green << 16) | (blue << 8) | 0xFF);
}
