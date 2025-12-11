/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:31:19 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/11 14:35:40 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void		loop_game(void *data);
static void		calc_fps(t_data *d);
static void		set_title(mlx_t *mlx, double delta_time);

/**
 * Initializes the game loop and sets up hooks for key and mouse events.
 * @param mlx Pointer to the MLX instance.
 * @param data Pointer to the game data structure.
 * @returns void; Starts the game loop and handles rendering and input.
 */
void	run_game(mlx_t *mlx, t_data *data)
{
	data->prev_mouse_x = -1;
	data->rect = 32;
	data->gif_portal->last = 0;
	data->gif_portal->current = 0;
	data->gif_portal->frame = 0;
	data->mouse_enabled = true;
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
	mlx_set_cursor_mode(mlx, MLX_MOUSE_HIDDEN);
	mlx_key_hook(mlx, &single_press_interactions, (void *)data);
	mlx_cursor_hook(mlx, &mouse_move, (void *)data);
	mlx_loop_hook(mlx, &loop_game, (void *)data);
	mlx_loop(mlx);
}

/**
 * Main game loop function called every frame.
 * Handles frame rate calculation, title updating, rendering, and player input.
 * @param data Pointer to the game data structure.
 */
static void	loop_game(void *data)
{
	t_data					*d;

	d = (t_data *)data;
	calc_fps(d);
	update_doors(d->level->doors, d->level);
	set_title(d->mlx, d->delta_time);
	draw_all(d);
	player_input(d);
}

/**
 * Calculates the time elapsed since the last frame to determine delta time.
 * Updates movement and turn speeds based on the delta time to ensure
 * consistent movement regardless of frame rate.
 * @param d Pointer to the game data structure.
 */
static void	calc_fps(t_data *d)
{
	struct timeval			current_time;
	static struct timeval	last_time;

	gettimeofday(&current_time, NULL);
	if (last_time.tv_sec != 0 || last_time.tv_usec != 0)
	{
		d->delta_time = (current_time.tv_sec - last_time.tv_sec)
			+ (current_time.tv_usec - last_time.tv_usec) / 1000000.0;
	}
	d->gif_portal->current
		= current_time.tv_sec + current_time.tv_usec / 1000000.0;
	last_time = current_time;
	d->move_speed = MOVESPEED * d->delta_time * 60;
	d->turn_speed = TURNSPEED * d->delta_time * 60;
	if (d->move_speed == 0)
		d->move_speed = MOVESPEED;
	if (d->turn_speed == 0)
		d->turn_speed = TURNSPEED;
}

/**
 * Sets the window title to include the current frames per second (FPS).
 * @param mlx Pointer to the MLX instance.
 * @param delta_time Time elapsed since the last frame.
 */
static void	set_title(mlx_t *mlx, double delta_time)
{
	char	title[50];
    char	*fps_str;

    ft_strlcpy(title, "cub3D | FPS: ", 50);
    fps_str = ft_itoa((int)(1.0 / delta_time));
    if (fps_str)
    {
        ft_strlcat(title, fps_str, 50);
        free(fps_str);
    }
    mlx_set_window_title(mlx, title);
}
