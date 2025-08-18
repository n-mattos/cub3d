/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:00:56 by nmattos           #+#    #+#             */
/*   Updated: 2025/08/18 13:53:29 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

uint32_t	get_pixel_color(mlx_texture_t *texture, int x, int y)
{
	uint8_t	*pixels;
	size_t	index;
	int		r;
	int		g;
	int		b;

	pixels = texture->pixels;
	index = (y * texture->width + x) * texture->bytes_per_pixel;
	r = pixels[index];
	g = pixels[index + 1];
	b = pixels[index + 2];
	return ((r << 24) | (g << 16) | (b << 8) | pixels[index + 3]);
}
