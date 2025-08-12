/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:59:36 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/12 12:25:02 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

/**
 * Draws the entire game frame, including the minimap and the main view.
 * @param data Pointer to the game data structure.
 */
void	draw_all(void *data)
{
	t_data	*d;

	d = (t_data *)data;
	mlx_t	*mlx = d->mlx;
	t_level	*level = d->level;
	if (!d->last_frame)
	{
		d->last_frame = mlx_new_image(mlx, 1920, 1080);
		mlx_image_to_window(mlx, d->last_frame, 0, 0);
	}
	if (!d->minimap)
	{
		d->minimap = mlx_new_image(mlx, 1920, 1080);
		mlx_image_to_window(mlx, d->minimap, 0, 0);
	}
	ft_memset(d->last_frame->pixels, 0, d->last_frame->width * d->last_frame->height * 4);
	ft_memset(d->minimap->pixels, 0, d->minimap->width * d->minimap->height * 4);
	draw_minimap(d);
	raycast_dda(level, d->minimap, d->last_frame);
	mlx_set_mouse_pos(d->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}
