/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/24 09:10:52 by nmattos       #+#    #+#                 */
/*   Updated: 2025/12/16 12:03:02 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

t_level	*parse_error(t_parse_tex_res parse_result)
{
	if (parse_result == TIS_SUCCESS)
		printf(MSG_ERR_BUT_SUCCESS);
	if (parse_result == TIS_TEXTURE_LOAD_FAIL)
		printf(MSG_ERR_TEX_LOAD_FAIL);
	if (parse_result == TIS_INVALID_COLOR_FORMAT)
		printf(MSG_ERR_WRONG_COLOR);
	if (parse_result == TIS_MALLOC_FAIL)
		printf(MSG_ERR_MALLOC_FAIL);
	if (parse_result == TIS_REDEFINE)
		printf(MSG_ERR_REDEFINE);
	if (parse_result == TIS_COLOR_MISSING)
		printf(MSG_ERR_COLOR_MISSING);
	return (NULL);
}

/**
 * Does the second half of parsing, blame norminette or making me split it up
 */
t_level	*continue_parse(t_textures *textures, char *map_str)
{
	t_level	*level;

	if (!map_str)
		return (free_textures(textures), printf(MSG_COMBINE_FAIL), NULL);
	level = parse_map(map_str);
	if (level == NULL)
		return (printf(MSG_ERR_PARSE_FAIL), free_textures(textures), NULL);
	level->textures = textures;
	level->player = NULL;
	if (!map_is_valid(level->map))
		return (printf(MSG_ERR_MAP_INVALID), free_map(level->map, -1),
			free_textures(textures), NULL);
	if (!get_portals(level->map, &level->portals))
		return (printf(MSG_ERR_PORTALS_INVALID), free_level(level), NULL);
	if (!get_doors(level->map, &level->doors))
		return (printf(MSG_ERR_DOORS_INVALID), free_level(level), NULL);
	level->player = retrieve_player(level->map);
	if (level->player == NULL)
		return (printf(MSG_ERR_NO_PLAYER), free_level(level), NULL);
	level->portal_effect_opacity = 0x00;
	return (level);
}

/**
 * Parses a .cub file to create a t_level structure.
 * @param fn_map The path to the .cub file.
 * @return A pointer to the t_level structure if successful, or NULL on failure.
 */
t_level	*parse(char *fn_map)
{
	t_textures		*tex;
	char			**lines;
	char			*map_str;
	t_parse_tex_res	res;

	lines = read_cub_file(fn_map);
	if (!lines)
		return (NULL);
	tex = new_parse_textures(lines);
	if (tex == NULL)
		return (free_2d_array(lines), printf(MSG_ERR_TEX_PARSE_FAIL), NULL);
	res = validate_parsed_textures(tex);
	if (res != TIS_SUCCESS)
		return (free_2d_array(lines), free_textures(tex), parse_error(res));
	if (!load_door_tex(tex) || !load_portal_tex(tex))
	{
		free_2d_array(lines);
		free_textures(tex);
		printf(MSG_ERR_STATIC_TEX_FAIL);
		return (NULL);
	}
	map_str = join_map_lines(tex, lines);
	free_2d_array(lines);
	return (continue_parse(tex, map_str));
}
