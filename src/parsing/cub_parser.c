/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:26:59 by mschippe          #+#    #+#             */
/*   Updated: 2025/12/13 17:19:10 by mschippe         ###   ########.fr       */
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
	close(fd);
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
			return (free_array((void **)arr, NULL), false);
	}
	arr[size] = NULL;
	return (true);
}
