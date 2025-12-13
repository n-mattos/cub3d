/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_parser5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:16:43 by mschippe          #+#    #+#             */
/*   Updated: 2025/12/13 17:37:33 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

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
		{
			if (update_tex_defined(get_tex_type(lines[i]), &tex->redef_check))
				break ;
			insert_tex(tex, get_tex_type(lines[i]), get_tex_value(lines[i]));
		}
		else
			break ;
		i++;
	}
	tex->tex_line_offset = i;
	return (tex);
}

t_parse_tex_res	is_redef_err(t_parse_tex_res res, t_tex_redef_check *redef)
{
	if (redef->north > 1
		|| redef->east > 1
		|| redef->south > 1
		|| redef->west > 1
		|| redef->floor > 1
		|| redef->ceiling > 1)
		return (TIS_REDEFINE);
	return (res);
}

t_parse_tex_res	validate_parsed_textures(t_textures *tex)
{
	if (!tex->north || !tex->east || !tex->south || !tex->west)
		return (is_redef_err(TIS_TEXTURE_LOAD_FAIL, &tex->redef_check));
	if (tex->floor == -2 || tex->ceiling == -2)
		return (is_redef_err(TIS_MALLOC_FAIL, &tex->redef_check));
	if (tex->floor == -3 || tex->ceiling == -3)
		return (is_redef_err(TIS_INVALID_COLOR_FORMAT, &tex->redef_check));
	if (tex->floor == -4 || tex->ceiling == -4)
		return (is_redef_err(TIS_INVALID_COLOR_FORMAT, &tex->redef_check));
	if (tex->floor == -5 || tex->ceiling == -5)
		return (is_redef_err(TIS_COLOR_MISSING, &tex->redef_check));
	return (is_redef_err(TIS_SUCCESS, &tex->redef_check));
}

/**
 * Stitches lines (back) together into one big map string because the map parsing
 * of old parsing should still suffice,
 * and this is the format it expects the map to be in
 */
char	*join_map_lines(t_textures *tex, char **lines)
{
	size_t	index;
	char	*res;
	char	*tmp;
	bool	last_is_nl;

	index = 0;
	last_is_nl = false;
	res = ft_calloc(1, sizeof(char));
	if (!res || !tex || !lines)
		return (free(res), NULL);
	while ((lines[index] && index < (size_t)tex->tex_line_offset)
		|| lines[index][0] == '\n')
		index++;
	while (lines[index++])
	{
		if (last_is_nl && lines[index - 1][0] != '\n')
			return (free(res), NULL);
		last_is_nl = (lines[index - 1][0] == '\n');
		tmp = ft_strjoin(res, lines[index - 1]);
		if (!tmp)
			return (free(res), NULL);
		free(res);
		res = tmp;
	}
	return (res);
}
