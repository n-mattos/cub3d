/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/26 15:06:01 by nmattos-         ###   ########.fr       */
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
		step = (t_point){raydir_x * 1000000 / speed, raydir_y * 1000000 / speed};	// bigger step size => faster raycasting, but less accurate

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

#define MAP_HEIGHT 14

const char *hardcoded_map[MAP_HEIGHT] = {
    "1111111111111111111111111",
    "1000000000110000000000001",
    "1011000001110000000000001",
    "100100000000000000000000111111111",
    "111111111011000001110000000000001",
    "100000000011000001110111110111111",
    "11110111111111011100000010001",
    "11110111111111011101010010001",
    "11000000110101011100000010001",
    "10000000000000001100000010001",
    "10000000000000001101010010001",
    "11000001110101011111011110N0111",
    "11110111 1110101 101111010001",
	"11111111 1111111 111111111111"
};

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
		return (0x12345678); //lol
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
	if (level)
	{
		int y = 0;
		if (level->map)
		{
			while (level->map[y])
			{
				free(level->map[y]);
				y++;
			}
			free(level->map);
		}
		free(level);
	}
	return (0);
}
