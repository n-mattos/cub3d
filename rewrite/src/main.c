/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/07/10 15:41:16 by nmattos-         ###   ########.fr       */
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
	raycast_dda(level, minimap);
	mlx_image_to_window(mlx, minimap, 0, 0);
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
