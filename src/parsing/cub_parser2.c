/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_parser2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:16:35 by mschippe          #+#    #+#             */
/*   Updated: 2025/12/13 17:18:35 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

char	**read_cub_file(char *fn)
{
	int		fd;
	int		size;
	char	**lines;

	if (!is_cub_file(fn))
		return (printf(MSG_ERR_NOT_CUB), NULL);
	size = get_line_count(fn);
	if (size == -1)
		return (printf(MSG_ERR_SIZE_CALC_FAIL), NULL);
	fd = open(fn, O_RDONLY);
	if (fd == -1)
		return (printf(MSG_ERR_FILE_OPEN_FAIL), NULL);
	if (size < 9)
		return (close(fd), printf(MSG_ERR_FILE_TOO_SMALL), NULL);
	lines = ft_calloc(size + 1, sizeof(char *));
	if (!lines)
		return (close(fd),
			printf(MSG_ERR_FILE_READ_MALLOC_FAIL), NULL);
	if (!read_into_array(lines, fd, size))
		return (close(fd),
			printf(MSG_ERR_FILE_SIZE_MISMATCH), NULL);
	close(fd);
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
