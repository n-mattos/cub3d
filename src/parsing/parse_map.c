/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/25 09:59:28 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static char	*get_raw_map_data(int fd);
static int	**create_map(t_level *level, char *raw_map);
static int	get_tile(char c);

t_level	*parse_map(int fd)
{
	t_level	*level;
	char	*raw_map;

	level = malloc(sizeof(t_level));
	if (level == NULL)
		return (NULL);
	raw_map = get_raw_map_data(fd);
	level->map = malloc(sizeof(int *) * (count_char(raw_map, '\n') + 2));
	if (level->map == NULL)
		return (free(level), NULL);
	level->map = create_map(level, raw_map);
	if (level->map == NULL)
		return (NULL);
	return (level);
}

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
	return(raw_map);
}

static int	**create_map(t_level *level, char *raw_map)
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
			return (free_map(level->map, y - 1), free(level), NULL);	// clean level->map[(x ∈ [0, y - 1])]
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

static int	get_tile(char c)
{
	if (is_whitespace(c))
		return (EMPTY);
	return (c);
}
