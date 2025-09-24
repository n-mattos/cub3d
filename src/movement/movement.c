/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:53:45 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/24 11:58:23 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	collision(t_level *level, t_vect new);
static void	turn(t_playerdata *p, double turnspeed);
static t_vect	move_longitudinal(t_data *d, t_playerdata *p);
static t_vect	move_lateral(t_data *d, t_playerdata *p);
static void	turn_keys(t_data *d, t_playerdata *p);

/**
 * Handles player movement based on key inputs.
 * @param keydata The key data containing the pressed key and its action.
 * @param data Pointer to the game data structure.
 * @returns void; Updates the player's position based on the key pressed.
 */
void	player_input(t_data *d)
{
	t_playerdata	*p;
	t_vect			long_vect;
	t_vect			lat_vect;

	p = d->level->player;
	turn_keys(d, p);
	long_vect = move_longitudinal(d, p);
	lat_vect = move_lateral(d, p);
	collision(d->level, (t_vect){p->x + long_vect.x + lat_vect.x, p->y + long_vect.y + lat_vect.y});
	if (mlx_is_key_down(d->mlx, MLX_KEY_EQUAL))
		if (d->rect < 48)
			d->rect += 2;
	if (mlx_is_key_down(d->mlx, MLX_KEY_MINUS))
		if (d->rect > 12)
			d->rect -= 2;
	if (mlx_is_key_down(d->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(d->mlx);
}

static t_vect	move_longitudinal(t_data *d, t_playerdata *p)
{
	t_vect	new;

	new = (t_vect){0, 0};
	if (mlx_is_key_down(d->mlx, MLX_KEY_W))
		new = (t_vect){new.x + p->dir_x * d->move_speed, new.y + p->dir_y * d->move_speed};
	if (mlx_is_key_down(d->mlx, MLX_KEY_S))
		new = (t_vect){new.x - p->dir_x * d->move_speed, new.y - p->dir_y * d->move_speed};
	return (new);
}

static t_vect	move_lateral(t_data *d, t_playerdata *p)
{
	t_vect	new;

	new = (t_vect){0, 0};
	if (mlx_is_key_down(d->mlx, MLX_KEY_A))
		new = (t_vect){new.x + p->dir_y * d->move_speed, new.y - p->dir_x * d->move_speed};
	if (mlx_is_key_down(d->mlx, MLX_KEY_D))
		new = (t_vect){new.x - p->dir_y * d->move_speed, new.y + p->dir_x * d->move_speed};
	return (new);
}

static void	turn_keys(t_data *d, t_playerdata *p)
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
	turn(data->level->player, -((IMG_WIDTH / 2 - x) * 0.0006));
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}

/**
 * Handles player collision with walls.
 * @param d Pointer to the game data structure.
 * @param p Pointer to the player data structure.
 * @param new_x The new x-coordinate of the player.
 * @param new_y The new y-coordinate of the player.
 * @returns void; Updates the player's position if the new coordinates
 * 				  do not collide with walls.
 */
static void	collision(t_level *level, t_vect new)
{
	t_playerdata	*p;

	p = level->player;
	if (new.y > p->y)
	{
		if (level->map[(int)(new.y + COLLISION_BUFFER)][(int)p->x] != WALL)
			p->y = new.y;
	}
	else
	{
		if (level->map[(int)(new.y - COLLISION_BUFFER)][(int)p->x] != WALL)
			p->y = new.y;
	}
	if (new.x < p->x)
	{
		if (level->map[(int)p->y][(int)(new.x - COLLISION_BUFFER)] != WALL)
			p->x = new.x;
	}
	else
	{
		if (level->map[(int)p->y][(int)(new.x + COLLISION_BUFFER)] != WALL)
			p->x = new.x;
	}
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
