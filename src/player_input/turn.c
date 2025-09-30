/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   turn.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: nmattos- <nmattos-@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/25 15:52:49 by nmattos-      #+#    #+#                 */
/*   Updated: 2025/09/30 13:46:55 by nmattos       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	turn(t_playerdata *p, double turnspeed);

/**
 * Rotates the player's direction and plane vectors based on the turn speed.
 * @param p Pointer to the player data structure.
 * @param turnspeed The speed at which the player turns.
 * @returns void; Updates the player's direction and plane vectors.
 */
void	turn_keys(t_data *d, t_playerdata *p)
{
	if (mlx_is_key_down(d->mlx, MLX_KEY_RIGHT))
		turn(p, d->turn_speed);
	if (mlx_is_key_down(d->mlx, MLX_KEY_LEFT))
		turn(p, -d->turn_speed);
}

/**
 * Handles mouse movement to turn the player.
 * @param x The x-coordinate of the mouse.
 * @param y The y-coordinate of the mouse. (Unused)
 * @param d Pointer to the game data structure.
 * @returns void; Updates the player's direction based on mouse movement.
 */
void	mouse_move(double x, double y, void *d)
{
	t_data	*data;

	(void)y;
	data = (t_data *)d;
	if (data->prev_mouse_x == -1)
	{
		data->prev_mouse_x = x;
		return ;
	}
	data->prev_mouse_x = x;
	turn(data->level->player, -((IMG_WIDTH / 2 - x) * 0.0004));
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}

/**
 * Rotates the player's direction and plane vectors based on the turn speed.
 * @param p Pointer to the player data structure.
 * @param turnspeed The speed at which the player turns.
 * @returns void; Updates the player's direction and plane vectors.
 */
static void	turn(t_playerdata *p, double turnspeed)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = p->dir_x;
	oldplane_x = p->plane_x;
	p->dir_x = p->dir_x * cos(turnspeed) - p->dir_y * sin(turnspeed);
	p->dir_y = olddir_x * sin(turnspeed) + p->dir_y * cos(turnspeed);
	p->plane_x = p->plane_x * cos(turnspeed) - p->plane_y * sin(turnspeed);
	p->plane_y = oldplane_x * sin(turnspeed) + p->plane_y * cos(turnspeed);
}
