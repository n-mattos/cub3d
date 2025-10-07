/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:13:46 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/07 17:04:10 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static mlx_t	*initialize_mlx(t_level *level);
static t_data	*allocate_data(t_level *level, mlx_t *mlx);

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	t_level		*level;
	t_data		*data;

	if (argc < 2)
		return (printf(USAGE), 1);
	level = parse(argv[1]);
	if (level == NULL)
		return (1);
	mlx = initialize_mlx(level);
	data = allocate_data(level, mlx);
	run_game(mlx, data);
	exit_program(mlx, data);
	return (0);
}

/**
 * Initializes the MLX instance with the specified width and height.
 * @param level Pointer to the game level structure (used for error handling).
 * @returns mlx_t*; Pointer to the initialized MLX instance.
 */
static mlx_t	*initialize_mlx(t_level *level)
{
	mlx_t	*mlx;

	mlx = mlx_init(IMG_WIDTH, IMG_HEIGHT, "cub3D", false);
	if (!mlx)
	{
		perror("Error\nFailed to initialize mlx");
		free_level(level);
		exit (1);
	}
	return (mlx);
}

/**
 * Allocates memory for the game data structure and initializes its members.
 * @param level Pointer to the game level structure.
 * @param mlx Pointer to the MLX instance.
 * @returns t_data*; Pointer to the allocated and
 * 	initialized game data structure.
 */
static t_data	*allocate_data(t_level *level, mlx_t *mlx)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
	{
		perror("Error\nFailed to allocate memory for game data");
		free_level(level);
		mlx_terminate(mlx);
		exit (1);
	}
	data->level = level;
	data->mlx = mlx;
	data->gif_portal = ft_calloc(1, sizeof(t_gif));
	if (!data->gif_portal)
	{
		perror("Error\nFailed to allocate memory for gif data");
		free_level(level);
		mlx_terminate(mlx);
		free(data);
		exit (1);
	}
	return (data);
}

/**
 * Frees all allocated resources and exits the program.
 */
void	exit_program(mlx_t *mlx, t_data *data)
{
	free_door_list(&data->level->doors);
	free_portal_list(&data->level->portals);
	free_level(data->level);
	free(data->gif_portal);
	free(data);
	mlx_terminate(mlx);
}
