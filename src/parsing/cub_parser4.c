/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_parser4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:16:41 by mschippe          #+#    #+#             */
/*   Updated: 2025/12/13 17:36:40 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	update_tex_defined(t_tex_info_type type, t_tex_redef_check *redef)
{
	if (type == TI_NORTH && !redef->north++)
		return (false);
	if (type == TI_EAST && !redef->east++)
		return (false);
	if (type == TI_SOUTH && !redef->south++)
		return (false);
	if (type == TI_WEST && !redef->west++)
		return (false);
	if (type == TI_FLOOR && !redef->floor++)
		return (false);
	if (type == TI_CEILING && !redef->ceiling++)
		return (false);
	return (true);
}

void	insert_tex(t_textures *tex, t_tex_info_type type, char *value)
{
	if (!value)
		return ;
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
	free(value);
}

char	*create_filename(char *pre, int num, char *post)
{
	char	*tmp;
	char	*part2;

	tmp = ft_itoa(num);
	if (!tmp)
		return (false);
	part2 = ft_strjoin(pre, tmp);
	free(tmp);
	if (!part2)
		return (false);
	tmp = ft_strjoin(part2, post);
	free(part2);
	return (tmp);
}

bool	load_portal_tex(t_textures *tex)
{
	int		index;
	char	*filename;

	index = 0;
	while (index < PORTAL_FRAME_COUNT)
	{
		filename = create_filename("textures/portal/", index + 1, ".png");
		if (!filename)
			return (false);
		tex->portal[index] = mlx_load_png(filename);
		free(filename);
		if (!tex->portal[index])
			return (false);
		index++;
	}
	return (true);
}

bool	load_door_tex(t_textures *tex)
{
	int		index;
	char	*filename;

	index = 0;
	while (index < DOOR_FRAME_COUNT)
	{
		filename = create_filename("textures/door/", index + 1, ".png");
		if (!filename)
			return (false);
		tex->door[index] = mlx_load_png(filename);
		free(filename);
		if (!tex->door[index])
			return (false);
		index++;
	}
	return (true);
}
