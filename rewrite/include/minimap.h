/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:31:00 by mschippe          #+#    #+#             */
/*   Updated: 2025/07/10 14:34:10 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H
# define RECT_SIZE 40
# define PLAYER_SIZE 8
# define WALL_COLOR 0xFF0000FF
# define EMPTY_COLOR 0x00000000
# define FLOOR_COLOR 0xFF00FF00
# define PLAYER_COLOR 0xFFFF0000
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