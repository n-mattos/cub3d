/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:49:41 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/11 14:35:51 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_vect	move_longitudinal(t_data *d, t_playerdata *p);
static t_vect	move_lateral(t_data *d, t_playerdata *p);

/**
 * Handles player movement based on key inputs.
 * @param d Pointer to the game data structure.
 * @param p Pointer to the player data structure.
 * @returns void; Updates the player's position based on the key pressed.
 */
void	move_player(t_data *d, t_playerdata *p)
{
	t_vect			long_vect;
	t_vect			lat_vect;

	if (mlx_is_key_down(d->mlx, MLX_KEY_LEFT_SHIFT))
		d->move_speed /= 2;
	long_vect = move_longitudinal(d, p);
	lat_vect = move_lateral(d, p);
	if ((long_vect.x != 0 || long_vect.y != 0)
		&& (lat_vect.x != 0 || lat_vect.y != 0))
	{
		long_vect.x *= HYPOTENUSE;
		long_vect.y *= HYPOTENUSE;
		lat_vect.x *= HYPOTENUSE;
		lat_vect.y *= HYPOTENUSE;
	}
	collision(d->level, (t_vect){p->x + long_vect.x + lat_vect.x,
		p->y + long_vect.y + lat_vect.y});
}

/**
 * Calculates the longitudinal movement vector based on key inputs.
 * @param d Pointer to the game data structure.
 * @param p Pointer to the player data structure.
 * @returns t_vect; The longitudinal movement vector.
 */
static t_vect	move_longitudinal(t_data *d, t_playerdata *p)
{
	t_vect	new;

	new = (t_vect){0, 0};
	if (mlx_is_key_down(d->mlx, MLX_KEY_W))
		new = (t_vect){new.x + p->dir_x * d->move_speed,
			new.y + p->dir_y * d->move_speed};
	if (mlx_is_key_down(d->mlx, MLX_KEY_S))
		new = (t_vect){new.x - p->dir_x * d->move_speed,
			new.y - p->dir_y * d->move_speed};
	return (new);
}

/**
 * Calculates the lateral movement vector based on key inputs.
 * @param d Pointer to the game data structure.
 * @param p Pointer to the player data structure.
 * @returns t_vect; The lateral movement vector.
 */
static t_vect	move_lateral(t_data *d, t_playerdata *p)
{
	t_vect	new;

	new = (t_vect){0, 0};
	if (mlx_is_key_down(d->mlx, MLX_KEY_A))
		new = (t_vect){new.x + p->dir_y * d->move_speed,
			new.y - p->dir_x * d->move_speed};
	if (mlx_is_key_down(d->mlx, MLX_KEY_D))
		new = (t_vect){new.x - p->dir_y * d->move_speed,
			new.y + p->dir_x * d->move_speed};
	return (new);
}
