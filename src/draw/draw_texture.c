/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:00:56 by nmattos           #+#    #+#             */
/*   Updated: 2025/10/02 17:04:53 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static mlx_texture_t	*get_texture(t_textures *textures, t_raycast *ray);

uint32_t	get_pixel_color(t_textures *textures, t_raycast *ray)
{
	uint8_t			*pixels;
	mlx_texture_t	*txt;
	size_t			index;

	txt = get_texture(textures, ray);
	pixels = txt->pixels;
	index = (ray->txt_y * txt->width + ray->txt_x) * txt->bytes_per_pixel;
	return ((pixels[index] << 24) | (pixels[index + 1] << 16)
		| (pixels[index + 2] << 8) | pixels[index + 3]);
}

static mlx_texture_t	*get_texture(t_textures *textures, t_raycast *ray)
{
	mlx_texture_t	*txt;
	uint8_t			pixel;

	txt = textures->portal[ray->frame];
	pixel = txt->pixels[((ray->txt_y * txt->width + ray->txt_x)
			* txt->bytes_per_pixel) + 3];
	if (ray->tile == PORTAL)
		if (pixel != 0)
			return (txt);
	txt = textures->door;
	pixel = txt->pixels[((ray->txt_y * txt->width + ray->txt_x)
			* txt->bytes_per_pixel) + 3];
	if (ray->tile == DOOR)
		return (txt);
	if (ray->hit_side == VERTICAL)
	{
		if (ray->raydir.x > 0)
			return (textures->east);
		return (textures->west);
	}
	if (ray->raydir.y > 0)
		return (textures->north);
	return (textures->south);
}
