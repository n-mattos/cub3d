/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:10:52 by nmattos           #+#    #+#             */
/*   Updated: 2025/12/13 14:22:02 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/*
	TIS_SUCCESS,
	TIS_TEXTURE_LOAD_FAIL,
	TIS_INVALID_COLOR_FORMAT,
	TIS_MALLOC_FAIL,
	TIS_REDEFINE,
	TIS_COLOR_MISSING,*/

t_level	*parse_error(t_parse_tex_res parse_result)
{
	if (parse_result == TIS_SUCCESS)
		perror("Error\nParsing was successful but error still thrown\n");
	if (parse_result == TIS_TEXTURE_LOAD_FAIL)
		perror("Error\nTexture(s) missing from file or failed to load\n");
	if (parse_result == TIS_INVALID_COLOR_FORMAT)
		perror("Error\nFloor or ceiling is formatted incorrectly\n");
	if (parse_result == TIS_MALLOC_FAIL)
		perror("Error\nA memory allocation failed during parsing\n");
	if (parse_result == TIS_REDEFINE)
		perror("Error\nAt least one field in the file was redefined\n");
	if (parse_result == TIS_COLOR_MISSING)
		perror("Error\nAt least one color is missing from the file\n");
	return (NULL);
}

/**
 * Parses a .cub file to create a t_level structure.
 * @param fn_map The path to the .cub file.
 * @return A pointer to the t_level structure if successful, or NULL on failure.
 */
t_level	*parse(char *fn_map)
{
	t_level			*level;
	t_textures		*textures;
	char			**lines;
	char			*map_str;
	t_parse_tex_res	parse_result;

	lines = read_cub_file(fn_map);
	if (!lines)
		return (NULL);
	textures = new_parse_textures(lines);
	if (textures == NULL)
		return (perror("Error\nFailed to parse textures"), NULL);
	parse_result = validate_parsed_textures(textures);
	if (parse_result != TIS_SUCCESS)
		return (parse_error(parse_result));
	if (!load_door_tex(textures) || !load_portal_tex(textures))
		return (perror("Error\nFailed to load static textures (door/portal)\n"), NULL);
	map_str = join_map_lines(textures, lines);
	if (!map_str)
		return (printf("Error\nMap combining failed\n"), NULL);
	level = parse_map(map_str);
	if (level == NULL)
		return (perror("Error\nmap == NULL"), free_textures(textures), NULL);
	level->textures = textures;
	level->player = NULL;
	if (!map_is_valid(level->map))
		return (perror("Error\nMap is invalid\n"), free_map(level->map, -1),
			free_textures(textures), NULL);
	if (!get_portals(level->map, &level->portals))
		return (perror("Error\nInvalid portals\n"), free_level(level), NULL);
	if (!get_doors(level->map, &level->doors))
		return (perror("Error\nInvalid doors\n"), free_level(level), NULL);
	level->player = retrieve_player(level->map);
	if (level->player == NULL)
		return (perror("Error\nNo player found"), free_level(level), NULL);
	level->portal_effect_opacity = 0x00;
	return (level);
}
