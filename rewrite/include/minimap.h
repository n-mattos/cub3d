/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:31:00 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/14 16:55:08 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H
# define RECT_SIZE 8
# define PLAYER_SIZE 2
# define WALL_COLOR 0xFF0000FF
# define EMPTY_COLOR 0x00000000
# define FLOOR_COLOR 0xFF00FF00
# define PLAYER_COLOR 0xFFFF0000
# define LINESMOOTHNESS 1000
#include "MLX42/MLX42.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <math.h>

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_vect
{
	double	x;
	double	y;
}	t_vect;

void	drawrectangle(mlx_image_t *img, t_point wh, t_point coord, uint32_t color);
#endif