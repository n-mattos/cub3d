/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:53:45 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 16:10:25 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	zoom_minimap(t_data *d);

/**
 * Handles player movement based on key inputs.
 * @param keydata The key data containing the pressed key and its action.
 * @param data Pointer to the game data structure.
 * @returns void; Updates the player's position based on the key pressed.
 */
void	player_input(t_data *d)
{
	move_player(d, d->level->player);
	turn_keys(d, d->level->player);
	zoom_minimap(d);
}

/**
 * Handles single press interactions like opening doors and exiting the game.
 * @param keydata The key data containing the pressed key and its action.
 * @param data Pointer to the game data structure.
 * @returns void; Performs actions based on the key pressed.
 */
void	single_press_interactions(mlx_key_data_t keydata, void *data)
{
	t_data	*d;

	d = (t_data *)data;
	door_interaction(d, keydata);
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(d->mlx);
}

/**
 * Handles minimap zooming based on key inputs.
 * @param d Pointer to the game data structure.
 * @returns void; Updates the minimap zoom level based on the key pressed.
 */
static void	zoom_minimap(t_data *d)
{
	if (mlx_is_key_down(d->mlx, MLX_KEY_EQUAL))
		if (d->rect < 48)
			d->rect += 2;
	if (mlx_is_key_down(d->mlx, MLX_KEY_MINUS))
		if (d->rect > 12)
			d->rect -= 2;
}
