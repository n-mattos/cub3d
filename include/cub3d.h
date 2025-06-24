/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:53:01 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/24 11:55:03 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CUB3D_H
# define CUB3D_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include "../libft/libft.h"
// include mlx library

typedef enum e_tile {
	EMPTY = ' ',
	WALL = '1',
	FLOOR = '0',
	NORTH = 'N',
	EAST = 'E',
	SOUTH = 'S',
	WEST = 'W'
}	t_tile;

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
	int			**map;
	t_textures	*textures;
}	t_level;

/*****************************************************************************\
*	Function Prototypes														  *
\*****************************************************************************/

/* /parsing */
t_level		*parse(char *fn_map);
t_textures	*parse_textures(int fd);
t_level		*parse_map(int fd);

#endif
