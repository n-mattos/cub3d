/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/27 08:52:54 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include "../include/minimap.h"

uint32_t getcolor(t_tile tile);
t_point	get_player_draw_loc(int x, int y);

void	draw_stuff(void *data)
{
	t_data	*d;

	d = (t_data *)data;
	mlx_t	*mlx = d->mlx;
	t_level	*level = d->level;
	mlx_image_t	*minimap = d->minimap;
	mlx_delete_image(mlx, minimap);
	minimap = mlx_new_image(mlx, 1920, 1080);
	if (level && level->map)
	{
		int y = 0;
		while (level->map[y])
		{
			int x = 0;
			while (level->map[y][x])
			{
				if (level->map[y][x] == WALL || level->map[y][x] == FLOOR)
					drawrectangle(minimap, (t_point){RECT_SIZE, RECT_SIZE}, (t_point){x * RECT_SIZE, y * RECT_SIZE}, getcolor(level->map[y][x]));
				else if (level->map[y][x] != EMPTY)
				{
					drawrectangle(minimap, (t_point){RECT_SIZE, RECT_SIZE}, (t_point){x * RECT_SIZE, y * RECT_SIZE}, FLOOR_COLOR);
					drawrectangle(minimap, (t_point){PLAYER_SIZE, PLAYER_SIZE}, get_player_draw_loc(x, y), getcolor(level->map[y][x]));
				}
				x++;
			}
			y++;
		}
	}
	single_ray(level, minimap, 177);
	single_ray(level, minimap, 180);
	single_ray(level, minimap, 185);
	single_ray(level, minimap, 190);
	single_ray(level, minimap, 195);
	mlx_image_to_window(mlx, minimap, 0, 0);
}

void	move_direction(t_level **level, double angle_deg, int speed)
{
	double delta_x;
	double delta_y;
	double angle_rad = ((*level)->player->degrees + angle_deg) * 3.14159265358979323846 / 180.0;
	double raydir_x = cos(angle_rad);
	double raydir_y = -sin(angle_rad);
	t_point step;
	if (fabs(raydir_x) < 1.e-10)
		delta_x = 0;
	else
		delta_x = fabs(1 / raydir_x);
	if (fabs(raydir_y) < 1.e-10)
		delta_y = 0;
	else
		delta_y = fabs(1 / raydir_y);
	step = (t_point){raydir_x * 1000000 / speed, raydir_y * 1000000 / speed};
	(*level)->player->x += step.x;
	(*level)->player->y += step.y;
}

void	move(mlx_key_data_t keydata, void *data)
{
	t_data	*d;

	d = (t_data *)data;
	if (keydata.key == MLX_KEY_W)
	{
		move_direction(&d->level, 180, 10);
	}
	else if (keydata.key == MLX_KEY_A)
	{
		move_direction(&d->level, 270, 10);
	}
	else if (keydata.key == MLX_KEY_S)
	{
		move_direction(&d->level, 0, 10);
	}
	else if (keydata.key == MLX_KEY_D)
	{
		move_direction(&d->level, 90, 10);
	}
	else if (keydata.key == MLX_KEY_LEFT)
	{
		d->level->player->degrees += 1;
		if (d->level->player->degrees < 0)
			d->level->player->degrees -= 360;
	}
	else if (keydata.key == MLX_KEY_RIGHT)
	{
		d->level->player->degrees -= 1;
		if (d->level->player->degrees >= 360)
			d->level->player->degrees += 360;
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
		return (PLAYER_COLOR);
	else
		return (0x12345678);
}

int	main(void)
{
	mlx_t		*mlx;
	mlx_image_t	*minimap;

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
	minimap = mlx_new_image(mlx, 1920, 1080);
	if (level && level->map)
	{
		int y = 0;
		while (level->map[y])
		{
			int x = 0;
			while (level->map[y][x])
			{
				if (level->map[y][x] == WALL || level->map[y][x] == FLOOR)
					drawrectangle(minimap, (t_point){RECT_SIZE, RECT_SIZE}, (t_point){x * RECT_SIZE, y * RECT_SIZE}, getcolor(level->map[y][x]));
				else if (level->map[y][x] != EMPTY)
				{
					drawrectangle(minimap, (t_point){RECT_SIZE, RECT_SIZE}, (t_point){x * RECT_SIZE, y * RECT_SIZE}, FLOOR_COLOR);
					drawrectangle(minimap, (t_point){PLAYER_SIZE, PLAYER_SIZE}, get_player_draw_loc(x, y), getcolor(level->map[y][x]));
				}
				x++;
			}
			y++;
		}
	}

	t_data *data = malloc(sizeof(t_data));
	data->level = level;
	data->mlx = mlx;
	data->minimap = minimap;
	draw_stuff(data);
	mlx_key_hook(mlx, &move, (void *)data);
	mlx_loop_hook(mlx, &draw_stuff, (void *)data);
	mlx_loop(mlx);
	mlx_terminate(mlx);

	free_level(level);
	mlx_delete_image(mlx, minimap);
	free(level->textures);
	free(data);
	return (0);
}
