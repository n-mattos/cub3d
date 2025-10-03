/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:19:59 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/03 13:55:59 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	create_background(t_data *d)
{
	if (!d->background)
	{
		d->background = mlx_new_image(d->mlx, IMG_WIDTH, IMG_HEIGHT);
		draw_floor_ceiling(d->background, d->level->textures);
		mlx_image_to_window(d->mlx, d->background, 0, 0);
	}
}

void	create_last_frame(t_data *d)
{
	if (!d->last_frame)
	{
		d->last_frame = mlx_new_image(d->mlx, IMG_WIDTH, IMG_HEIGHT);
		mlx_image_to_window(d->mlx, d->last_frame, 0, 0);
	}
}

void	create_minimap(t_data *d)
{
	if (!d->minimap)
	{
		d->minimap = mlx_new_image(d->mlx, 400, 400);
		mlx_image_to_window(d->mlx, d->minimap, 0, 0);
	}
}

void	create_portal_effect(t_data *d)
{
	if (!d->portal_effect)
	{
		d->portal_effect = mlx_new_image(d->mlx, IMG_WIDTH, IMG_HEIGHT);
		mlx_image_to_window(d->mlx, d->portal_effect, 0, 0);
	}
}

void create_crosshair(t_data *d)
{
    if (!d->crosshair)
    {
		mlx_texture_t	*texture;

		texture = mlx_load_png("textures/crosshair.png");
		if (!texture)
			return ;
		d->crosshair = mlx_new_image(d->mlx, texture->width, texture->height);
		if (!d->crosshair)
		{
			mlx_delete_texture(texture);
			return ;
		}
		ft_memcpy(d->crosshair->pixels, texture->pixels,
				texture->width * texture->height * 4);
		mlx_delete_texture(texture);
        if (d->crosshair)
        {
            mlx_image_to_window(d->mlx, d->crosshair,
                (IMG_WIDTH / 2) - (16 / 2),
                (IMG_HEIGHT / 2) - (16 / 2));
        }
    }
}
