/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/07 16:55:00 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static char		*get_raw_map_data(int fd);
static int		**create_map(t_level *level, char *raw_map);
static size_t	calc_longest(char *str);
static int		get_tile(char c);

/**
 * Parses the map from a file descriptor and returns a t_level structure.
 * @param fd The file descriptor to read the map from.
 * @return A pointer to a t_level structure containing the parsed map,
 *         or NULL if an error occurs.
 */
t_level	*parse_map(int fd)
{
	t_level	*level;
	char	*raw_map;

	level = malloc(sizeof(t_level));
	if (level == NULL)
		return (NULL);
	level->portals = NULL;
	level->doors = NULL;
	raw_map = get_raw_map_data(fd);
	level->map = malloc(sizeof(int *) * (count_char(raw_map, '\n') + 2));
	if (level->map == NULL)
	{
		close(fd);
		return (free(level), NULL);
	}
	level->map = create_map(level, raw_map);
	if (level->map == NULL)
	{
		close(fd);
		return (NULL);
	}
	close(fd);
	return (level);
}

/**
 * Reads the entire map from the file descriptor and
 * returns it as a single string.
 * @param fd The file descriptor to read from.
 * @return A string containing the raw map data, or NULL if an error occurs.
 */
static char	*get_raw_map_data(int fd)
{
	char	*line;
	char	*raw_map;
	char	*temp;

	raw_map = get_next_line(fd);
	line = get_next_line(fd);
	while (line)
	{
		temp = ft_strjoin(raw_map, line);
		free(raw_map);
		free(line);
		if (temp == NULL)
			return (NULL);
		raw_map = temp;
		line = get_next_line(fd);
	}
	return (raw_map);
}

/**
 * Creates a 2D int array from the raw map data.
 * @param level The t_level structure to populate with the map.
 * @param raw_map The raw map data as a string.
 * @return A pointer to the 2D int array representing the map,
 *         or NULL if an error occurs.
 */
static int	**create_map(t_level *level, char *raw_map)
{
	size_t	i;
	size_t	x;
	size_t	y;
	size_t	max_length;

	i = 0;
	y = 0;
	max_length = calc_longest(raw_map);
	while (raw_map[i] != '\0')
	{
		x = 0;
		level->map[y] = malloc(sizeof(int) * (max_length + 1));
		if (level->map[y] == NULL)
			return (free_map(level->map, y - 1), free(level), NULL);
		while (raw_map[i] != '\n' && raw_map[i] != '\0')
			level->map[y][x++] = get_tile(raw_map[i++]);
		while (x < max_length)
			level->map[y][x++] = EMPTY;
		level->map[y++][x] = '\0';
		if (raw_map[i] == '\n')
			i++;
	}
	free(raw_map);
	level->map[y] = NULL;
	return (level->map);
}

static size_t	calc_longest(char *str)
{
	size_t	longest;
	size_t	current;

	longest = 0;
	current = 0;
	while (*str != '\0')
	{
		if (*str == '\n')
		{
			if (current > longest)
				longest = current;
			current = 0;
		}
		else
			current++;
		str++;
	}
	if (current > longest)
		longest = current;
	return (longest);
}

/**
 * Converts a character to a tile value.
 * @param c The character to convert.
 * @return The corresponding tile value.
 */
static int	get_tile(char c)
{
	if (is_whitespace(c))
		return (EMPTY);
	return (c);
}
