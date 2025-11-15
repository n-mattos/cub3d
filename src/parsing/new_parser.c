/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:26:59 by mschippe          #+#    #+#             */
/*   Updated: 2025/10/27 19:27:05 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	free_array(void **arr, void (*del)(void *))
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i] != NULL)
	{
		if (del)
			del(arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}

bool	cub_strcmp(char *s1, char *s2)
{
	if (!s1 && !s2)
		return (true);
	if (!s1 || !s2)
		return (false);
	while (*s1++ == *s2++)
		if (!*s1 && !*s2)
			return (true);
	return (false);
}

bool	is_cub_file(char *fn)
{
	int	len;

	len = ft_strlen(fn);
	if (len < 4)
		return (false);
	return (cub_strcmp(fn + len - 4, ".cub"));
}

int	get_line_count(char *fn)
{
	int		fd;
	int		count;
	char	*line;

	count = 0;
	fd = open(fn, O_RDONLY);
	if (fd == -1)
		return (-1);
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
		count++;
	}
	free(line);
	return (count);
}

bool	read_into_array(char **arr, int fd, int size)
{
	int		index;
	char	*line;
	
	index = 0;
	line = get_next_line(fd);
	while (line)
	{
		arr[index++] = line;
		line = get_next_line(fd);
		if (line && index == size)
			return(free_array((void **)arr, NULL), false);
	}
	arr[size] = NULL;
	return (true);
}

char	**read_cub_file(char *fn)
{
	int		fd;
	int		size;
	char	**lines;

	if (!is_cub_file(fn))
		return (perror("Error\nFile is not a .cub file\n"), NULL);
	size = get_line_count(fn);
	fd = open(fn, O_RDONLY);
	if (fd == -1 || size == -1)
		return (perror("Error\nFailed to open .cub file\n"), NULL);
	if (size < 9)
		return (perror("Error\nFile too small to be valid\n"), NULL);
	lines = ft_calloc(size + 1, sizeof(char *));
	if (!lines)
		return(perror("Error\nMemory alloc failed for file read\n"), NULL);
	if (!read_into_array(lines, fd, size))
		return (perror("Error\nFound more lines than expected\n"), NULL);
	return (lines);
}

size_t	num_spaces(char *str)
{
	size_t	spaces;

	spaces = 0;
	while (str && is_whitespace(*str++))
		spaces++;
	return (spaces);
}

bool	is_info_line(char *line)
{
	line += num_spaces(line);
	return ((!ft_strncmp(line, "NO ", 3)
		|| !ft_strncmp(line, "EA ", 3)
		|| !ft_strncmp(line, "SO ", 3)
		|| !ft_strncmp(line, "WE ", 3)
		|| !ft_strncmp(line, "F ", 2)
		|| !ft_strncmp(line, "C ", 2)));
}

bool	is_map_char(char c)
{
	return ((ft_isalpha(c) && c > 90 && c != 'd')
		|| is_whitespace(c)
		|| c == '0'
		|| c == '1'
		|| c == 'D'
		|| c == 'N'
		|| c == 'E'
		|| c == 'S'
		|| c == 'W');
}

bool	is_map_line(char *line)
{
	while (is_map_char(*line++))
		continue ;
	return (*line == '\0');
}

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
		continue;
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
		return (-2); //malloc fail, can NOT use -1 as that is a possible valid RGB int
	if (ft_arrlen((void **)split) != 3)
		return (-3); // invalid format
	if (!rgb_atoi(split[0], &red) || !rgb_atoi(split[1], &green) || !rgb_atoi(split[2], &blue))
		return (-4); // not a number or not within 0-255 range
	return ((red << 24) | (green << 16) | (blue << 8) | 0xFF);
}

void	insert_tex(t_textures *tex, t_tex_info_type type, char *value)
{
	if (type == TI_NORTH)
		tex->north = mlx_load_png(value);
	else if (type == TI_EAST)
		tex->east = mlx_load_png(value);
	else if (type == TI_SOUTH)
		tex->south = mlx_load_png(value);
	else if (type == TI_WEST)
		tex->west = mlx_load_png(value);
	else if (type == TI_FLOOR)
		tex->floor = string_to_rgb(value);
	else if (type == TI_CEILING)
		tex->ceiling = string_to_rgb(value);
}

t_textures	*new_parse_textures(char **lines)
{
	t_textures	*tex;
	int			i;

	i = 0;
	tex = allocate_textures();
	if (!tex)
		return (NULL);
	while (lines && lines[i])
	{
		if (*(lines[i]) == '\n' && lines[i++] != NULL)
			continue ;
		if (is_info_line(lines[i]))
			insert_tex(tex, get_tex_type(lines[i]), get_tex_value(lines[i]));
		else
			break ;
		i++;
	}
	return (tex);
}

t_parse_tex_res	validate_parsed_textures(t_textures *tex)
{
	if (!tex->north || !tex->east || !tex->south || !tex->west)
		return (TIS_TEXTURE_LOAD_FAIL);
	if (tex->floor == -2 || tex->ceiling == -2)
		return (TIS_MALLOC_FAIL);
	if (tex->floor == -3 || tex->ceiling == -3)
		return (TIS_INVALID_COLOR_FORMAT);
	if (tex->floor == -4 || tex->ceiling == -4)
		return (TIS_INVALID_COLOR_FORMAT);
	return (TIS_SUCCESS);
}
