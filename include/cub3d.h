/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:53:01 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/26 13:45:11 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CUB3D_H
# define CUB3D_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "../libft/libft.h"

typedef enum e_tile {
	EMPTY = ' ',
	WALL = '1',
	FLOOR = '0',
	NORTH = 'N',
	EAST = 'E',
	SOUTH = 'S',
	WEST = 'W'
}	t_tile;

typedef struct s_playerdata {
	size_t	x;
	size_t	y;
	double	degrees;
}	t_playerdata;

typedef struct s_textures {
	/*	mlx textures (?)
	north;
	east;
	south;
	west;
	*/
	char	*north;
	char	*east;
	char	*south;
	char	*west;
	char	*floor;
	char	*ceiling;
}	t_textures;

typedef struct s_level {
	int				**map;
	t_textures		*textures;
	t_playerdata	*player;
}	t_level;

/*****************************************************************************\
*	Function Prototypes														  *
\*****************************************************************************/

/* parse */
t_level		*parse(char *fn_map);
t_textures	*parse_textures(int fd);
t_level		*parse_map(int fd);
bool		map_is_valid(int **map);

/* parse_memory */
void		free_raw_textures(char **raw_textures);
void		free_textures(t_textures *textures);
void		free_map(int **map, size_t i);
void		free_level(t_level *level);
t_textures	*allocate_textures(void);

/* player */
t_playerdata	*retrieve_player(int **map);

/* utils */
size_t	chars_till_eol(char *str);
bool	is_whitespace(char c);
int		skip_whitespaces(char *str, int i);
size_t	count_char(char *str, char c);
bool	is_player(int c);

#endif
