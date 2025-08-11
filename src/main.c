/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/11 14:03:31 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	move(mlx_key_data_t keydata, void *data)
{
	t_data			*d;
	t_playerdata	*p;

	d = (t_data *)data;
	p = d->level->player;
	if (keydata.key == MLX_KEY_W)
	{
		p->x += p->dir_x * MOVESPEED;
		p->y += p->dir_y * MOVESPEED;
	}
	else if (keydata.key == MLX_KEY_A)
	{
		p->x += p->dir_y * MOVESPEED;
		p->y -= p->dir_x * MOVESPEED;
	}
	else if (keydata.key == MLX_KEY_S)
	{
		p->x -= p->dir_x * MOVESPEED;
		p->y -= p->dir_y * MOVESPEED;
	}
	else if (keydata.key == MLX_KEY_D)
	{
		p->x -= p->dir_y * MOVESPEED;
		p->y += p->dir_x * MOVESPEED;
	}
	else if (keydata.key == MLX_KEY_RIGHT)
	{
		double olddir_x = p->dir_x;
		p->dir_x = p->dir_x * cos(TURNSPEED) - p->dir_y * sin(TURNSPEED);
		p->dir_y = olddir_x * sin(TURNSPEED) + p->dir_y * cos(TURNSPEED);
		double oldplane_x = p->plane_x;
		p->plane_x = p->plane_x * cos(TURNSPEED) - p->plane_y * sin(TURNSPEED);
		p->plane_y = oldplane_x * sin(TURNSPEED) + p->plane_y * cos(TURNSPEED);
	}
	else if (keydata.key == MLX_KEY_LEFT)
	{
		double olddir_x = p->dir_x;
		p->dir_x = p->dir_x * cos(-TURNSPEED) - p->dir_y * sin(-TURNSPEED);
		p->dir_y = olddir_x * sin(-TURNSPEED) + p->dir_y * cos(-TURNSPEED);
		double oldplane_x = p->plane_x;
		p->plane_x = p->plane_x * cos(-TURNSPEED) - p->plane_y * sin(-TURNSPEED);
		p->plane_y = oldplane_x * sin(-TURNSPEED) + p->plane_y * cos(-TURNSPEED);
	}
	else if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(d->mlx);
	else
	{
		printf("Key %d pressed\n", keydata.key);
	}
}

void	mouse_move(double x, double y, void *d)
{
	t_data			*data;
	t_playerdata	*p;
	double			x_delta;

	(void)y;
	data = (t_data *)d;
	p = data->level->player;
	if (data->prev_mouse_x == -1)
	{
		data->prev_mouse_x = x;
		return ;
	}
	x_delta = IMG_WIDTH / 2 - x;
	data->prev_mouse_x = x;
	double rotspeed = -(x_delta * 0.0006);
	double olddir_x = p->dir_x;
	p->dir_x = p->dir_x * cos(rotspeed) - p->dir_y * sin(rotspeed);
	p->dir_y = olddir_x * sin(rotspeed) + p->dir_y * cos(rotspeed);
	double oldplane_x = p->plane_x;
	p->plane_x = p->plane_x * cos(rotspeed) - p->plane_y * sin(rotspeed);
	p->plane_y = oldplane_x * sin(rotspeed) + p->plane_y * cos(rotspeed);
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
}

void	mlx_setup(mlx_t *mlx, t_data *data)
{
	mlx_key_hook(mlx, &move, (void *)data);
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
	mlx_set_cursor_mode(mlx, MLX_MOUSE_HIDDEN);
	mlx_loop_hook(mlx, &draw_all, (void *)data);
	mlx_cursor_hook(mlx, &mouse_move, (void *)data);
	mlx_loop(mlx);
	mlx_delete_image(mlx, data->minimap);
}

int	main(void)
{
	mlx_t		*mlx;

	t_level *level = parse("maps/subject_example1.cub");
	if (level == NULL)
		return (1);
	printf("Texture struct:\n");
	printf("NO: %s\n", level->textures->north);
	printf("SO: %s\n", level->textures->south);
	printf("WE: %s\n", level->textures->west);
	printf("EA: %s\n", level->textures->east);
	printf("F: %s\n", level->textures->floor);
	printf("C: %s\n", level->textures->ceiling);

	mlx = mlx_init(1920, 1080, "CUB3D", false);
	if (!mlx)
	{
		fprintf(stderr, "Failed to initialize mlx\n");
		free_level(level);
		return (1);
	}

	t_data *data = ft_calloc(1, sizeof(t_data));
	data->level = level;
	data->mlx = mlx;
	data->prev_mouse_x = -1;

	draw_all(data);

	mlx_setup(mlx, data);

	free_level(level);
	mlx_terminate(mlx);
	// free(level->textures);
	free(data);
	return (0);
}
