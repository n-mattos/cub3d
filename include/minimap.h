/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:31:00 by mschippe          #+#    #+#             */
/*   Updated: 2025/06/23 15:24:48 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H
# define RECT_SIZE 20
# define PLAYER_SIZE 2
# define WALL_COLOR 0xFF0000FF
# define EMPTY_COLOR 0x00000000
# define FLOOR_COLOR 0xFF00FF00
# define PLAYER_COLOR 0xFFFF0000
#include "MLX42/MLX42.h"
#include "libft/libft.h"
#include <stdlib.h>

typedef struct s_point
{
	uint32_t	x;
	uint32_t	y;
}	t_point;
#endif