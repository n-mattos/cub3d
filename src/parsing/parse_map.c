/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/24 13:54:51 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

size_t	count_newlines(char *str)
{
	int		i;
	size_t	n;

	i = 0;
	n = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
			n++;
		i++;
	}
	if (i == 0)
		n = 0;
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

bool	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char	*get_raw_map_data(int fd)
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
	return(raw_map);
}

void	clean_map(int **map, size_t i)
{
	while (i >= 0)
		free(map[i--]);
	free(map);
}

int	get_tile(char c)
{
	if (is_whitespace(c))
		return (EMPTY);
	return (c);
}

int	**create_map(t_level *level, char *raw_map)
{
	size_t	i;
	size_t	x;
	size_t	y;

	i = 0;
	y = 0;
	while (raw_map[i] != '\0')
	{
		x = 0;
		level->map[y] = malloc(sizeof(int) * (chars_till_eol(&raw_map[i])) + 1);
		if (level->map[y] == NULL)
			return (clean_map(level->map, y - 1), free(level), NULL);	// clean level->map[(x ∈ [0, y - 1])]
		while (raw_map[i] != '\n' && raw_map[i] != '\0')
			level->map[y][x++] = get_tile(raw_map[i++]);
		level->map[y++][x] = '\0';
		if (raw_map[i] == '\n')
			i++;
	}
	free(raw_map);
	level->map[y] = NULL;
	return (level->map);
}

t_level	*parse_map(int fd)
{
	t_level	*level;
	char	*raw_map;

	level = malloc(sizeof(t_level));
	if (level == NULL)
		return (NULL);
	raw_map = get_raw_map_data(fd);
	level->map = malloc(sizeof(int *) * (count_newlines(raw_map) + 2));
	if (level->map == NULL)
		return (free(level), NULL);
	level->map = create_map(level, raw_map);
	if (level->map == NULL)
		return (NULL);
	return (level);
}
