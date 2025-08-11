/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/07/29 12:16:08 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include "../include/minimap.h"

uint32_t getcolor(t_tile tile);
t_point	get_player_draw_loc(int x, int y);

void	draw_minimap(t_data *d)
{
	int	y;

	y = 0;
	while (d->level->map[y])
	{
		int x = 0;
		while (d->level->map[y][x])
		{
			drawrectangle(d->minimap, (t_point){RECT_SIZE, RECT_SIZE}, (t_point){x * RECT_SIZE, y * RECT_SIZE}, getcolor(d->level->map[y][x]));
			x++;
		}
		y++;
	}
}

void	draw_stuff(void *data)
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

void	drawrectangle(mlx_image_t *img, t_point wh, t_point coord, uint32_t color)
{
	uint32_t	*pixels;
	int	x;
	int	y;

	x = 0;
	y = 0;
	pixels = (uint32_t *)(img->pixels);
	while (y < wh.y)
	{
		while (x < wh.x)
		{
			pixels[(y + coord.y) * img->width + (x + coord.x)] = color;
			x++;
		}
		x = 0;
		y++;
	}
}
t_point	get_player_draw_loc(int x, int y)
{
	return ((t_point){(x * RECT_SIZE) + (RECT_SIZE / 2 - (PLAYER_SIZE / 2 - 1)),
			(y * RECT_SIZE) + (RECT_SIZE / 2 - (PLAYER_SIZE / 2 - 1))});
}

uint32_t getcolor(t_tile tile)
{
	if (tile == FLOOR)
		return (FLOOR_COLOR);
	if (tile == EMPTY)
		return (EMPTY_COLOR);
	if (tile == WALL)
		return (WALL_COLOR);
	if (tile == NORTH || tile == EAST || tile == SOUTH || tile == WEST)
		return (FLOOR_COLOR);
	else
		return (0x12345678);
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

	t_data *data = ft_calloc(1, sizeof(t_data));
	data->level = level;
	data->mlx = mlx;
	data->prev_mouse_x = -1;
	draw_stuff(data);

	mlx_key_hook(mlx, &move, (void *)data);
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
	mlx_set_cursor_mode(mlx, MLX_MOUSE_HIDDEN);
	mlx_loop_hook(mlx, &draw_stuff, (void *)data);
	mlx_cursor_hook(mlx, &mouse_move, (void *)data);
	mlx_loop(mlx);
	mlx_delete_image(mlx, data->minimap);

	free_level(level);
	mlx_terminate(mlx);
	// free(level->textures);
	free(data);
	return (0);
}
