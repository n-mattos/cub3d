/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw_texture.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos <nmattos@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 11:00:56 by nmattos       #+#    #+#                 */
/*   Updated: 2025/08/14 11:01:00 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

uint32_t	get_pixel_color(mlx_texture_t *texture, int x, int y)
{
	uint8_t *pixels = texture->pixels;
	size_t index = (y * texture->width + x) * texture->bytes_per_pixel;
	int r = pixels[index];
	int g = pixels[index + 1];
	int b = pixels[index + 2];
	int a = pixels[index + 3];
	return (r << 24) | (g << 16) | (b << 8) | a;
}
