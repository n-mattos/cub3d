/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:00:56 by nmattos           #+#    #+#             */
/*   Updated: 2025/10/03 12:06:25 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static mlx_texture_t	*get_texture(t_level *level, t_raycast *ray);

uint32_t	get_pixel_color(t_level *level, t_raycast *ray)
{
	uint8_t			*pixels;
	mlx_texture_t	*txt;
	size_t			index;

	txt = get_texture(level, ray);
	pixels = txt->pixels;
	index = (ray->txt_y * txt->width + ray->txt_x) * txt->bytes_per_pixel;
	return ((pixels[index] << 24) | (pixels[index + 1] << 16)
		| (pixels[index + 2] << 8) | pixels[index + 3]);
}

static mlx_texture_t	*get_texture(t_level *level, t_raycast *ray)
{
	mlx_texture_t	*txt;
	uint8_t			pixel;

	txt = level->textures->portal[ray->frame];
	pixel = txt->pixels[((ray->txt_y * txt->width + ray->txt_x)
			* txt->bytes_per_pixel) + 3];
	if (ray->tile == PORTAL)
		if (pixel != 0)
			return (txt);
	if (ray->tile == DOOR || ray->tile == DOOR_OPEN)
	{
		txt = get_door_texture(level->doors, ray->map);
		pixel = txt->pixels[((ray->txt_y * txt->width + ray->txt_x)
				* txt->bytes_per_pixel) + 3];
		return (txt);
	}
	if (ray->hit_side == VERTICAL)
	{
		if (ray->raydir.x > 0)
			return (level->textures->east);
		return (level->textures->west);
	}
	if (ray->raydir.y > 0)
		return (level->textures->north);
	return (level->textures->south);
}
