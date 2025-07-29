/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:53:01 by nmattos-          #+#    #+#             */
/*   Updated: 2025/07/29 12:07:33 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include "MLX42/MLX42.h"
# include "../libft/libft.h"

# define TURNSPEED 0.05		// radians
# define MOVESPEED 0.1
# define TOTAL_RAYS 1920	// total rays to cast (width of the screen)
# define PERCENTAGE_RAYS 10	// percentage of rays to display (minimap)
# define IMG_HEIGHT 1080.0
# define IMG_WIDTH 1920.0
# define PI 3.14159265358979323846

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
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
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

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*minimap;
	mlx_image_t	*last_frame;
	t_level		*level;
	double		prev_mouse_x;
}	t_data;

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
void		free_map(int **map, int i);
void		free_level(t_level *level);
t_textures	*allocate_textures(void);

/* player */
t_playerdata	*retrieve_player(int **map);

/* raycast */
void	raycast_dda(t_level *lvl, mlx_image_t *mmap, mlx_image_t *frame);

/* utils */
size_t	chars_till_eol(char *str);
bool	is_whitespace(char c);
int		skip_whitespaces(char *str, int i);
size_t	count_char(char *str, char c);
bool	is_player(int c);

#endif
