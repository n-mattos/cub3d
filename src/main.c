/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/25 12:32:17 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/**
 * Initializes the game loop and sets up hooks for key and mouse events.
 * @param mlx Pointer to the MLX instance.
 * @param data Pointer to the game data structure.
 * @returns void; Starts the game loop and handles rendering and input.
 */
static void	run_game(mlx_t *mlx, t_data *data)
{
	mlx_key_hook(mlx, &move, (void *)data);
	mlx_set_mouse_pos(data->mlx, IMG_WIDTH / 2, IMG_HEIGHT / 2);
	mlx_set_cursor_mode(mlx, MLX_MOUSE_HIDDEN);
	mlx_loop_hook(mlx, &draw_all, (void *)data);
	mlx_cursor_hook(mlx, &mouse_move, (void *)data);
	mlx_loop(mlx);
	mlx_delete_image(mlx, data->minimap);
}

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	t_level		*level;
	t_data		*data;

	if (argc < 2)
		return (printf("Needs a map filename as arg1 pls\n"), 1);
	level = parse(argv[1]);
	if (level == NULL)
		return (1);
	mlx = mlx_init(1920, 1080, "cub3D", false);
	if (!mlx)
	{
		fprintf(stderr, "Failed to initialize mlx\n");
		free_level(level);
		return (1);
	}
	data = ft_calloc(1, sizeof(t_data));
	data->level = level;
	data->mlx = mlx;
	data->prev_mouse_x = -1;
	run_game(mlx, data);
	free_level(level);
	mlx_terminate(mlx);
	free(data);
	return (0);
}
