/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:09:25 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/03 14:08:44 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static t_raycast	door_ray(t_data *d);

/**
 * Handles door interaction.
 * @param d Pointer to the game data structure.
 * @param keydata The key data containing the pressed key and its action.
 * @returns void; Opens or closes doors if the player is facing one.
 */
void	door_interaction(t_data *d, mlx_key_data_t keydata)
{
	t_raycast		ray;
	t_playerdata	*p;

	p = d->level->player;
	ray = door_ray(d);
	if ((ray.tile == DOOR || ray.tile == DOOR_OPEN)
		&& ray.perp_wall_dist < 1.5
		&& d->level->map[(int)p->x][(int)p->y] != DOOR_OPEN)
	{
		if (keydata.key == MLX_KEY_E && keydata.action == MLX_PRESS)
			trigger_door(d->level, ray.map);
	}
}

/**
 * Casts a ray to check whether the player is facing a door.
 * @param d Pointer to the game data structure.
 * @returns t_raycast; The raycast data containing information about the door.
 */
static t_raycast	door_ray(t_data *d)
{
	t_raycast		ray;

	ray = single_ray(d, *d->level->player, (int)(d->last_frame->width / 2));
	calculate_ray((&ray.map), &ray, d->level, true);
	ray.perp_wall_dist = calculate_perpendicular_distance(
			*d->level->player, &ray, ray.map);
	return (ray);
}
