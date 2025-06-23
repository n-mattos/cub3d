/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:14:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/23 16:04:48 by nmattos-         ###   ########.fr       */
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

size_t	chars_till_eol(char *str)
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
			return (NULL);	// clean level
	int	i = 0;
	int	y = 0;
	int	x = 0;
	while (raw_map[i] != '\0')
	{
		level->map[y] = malloc(sizeof(int) * (chars_till_eol(&raw_map[i])) + 1);
		if (level->map[y] == NULL)
			return (NULL);	// clean level->map[(x ∈ [0, y - 1])]	+	clean level
		while (raw_map[i] != '\n' && raw_map[i] != '\0')
		{
			if (is_whitespace(raw_map[i]))
				level->map[y][x] = EMPTY;
			else
				level->map[y][x] = raw_map[i];
			x++;
			i++;
		}
		level->map[y][x] = '\0';
		if (raw_map[i] != '\0')
			i++;	// skip newline char
		y++;	// continue to next line
		x = 0;	// start at first element
	}
	free(raw_map);
	level->map[y] = NULL;
	return (level);
}
