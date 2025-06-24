/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rttest.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:58:15 by mschippe          #+#    #+#             */
/*   Updated: 2025/06/24 16:52:05 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include "../../include/minimap.h"

#define PI 3.14159265358979323846
#define ZERO 1.e-10

bool	isplayerletter(int l)
{
	return (l == 'N' || l == 'E' || l == 'S' || l == 'W');
}

t_point	findplayer(t_level *level)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (level && level->map[y])
	{
		while (level->map[y][x])
		{
			if (isplayerletter(level->map[y][x]))
				return ((t_point){x * 1000000, y * 1000000});
			x++;
		}
		x = 0;
		y++;
	}
	return ((t_point){-1, -1});
}

void	single_ray(t_level *lvl, mlx_image_t *img)
{
	double delta_x;
	double delta_y;
	double angle_deg = 195;				// 90
	double angle_rad = angle_deg * PI / 180.0;	// pi
	double raydir_x = cos(angle_rad); // 0
	double raydir_y = -sin(angle_rad); // 1
	t_point step;
	if (fabs(raydir_x) < ZERO)
		delta_x = 0;
	else
		delta_x = fabs(1 / raydir_x);
	if (fabs(raydir_y) < ZERO)
		delta_y = 0;
	else
		delta_y = fabs(1 / raydir_y);
	bool hit = false;
	t_point player = findplayer(lvl);
	step = (t_point){raydir_x * 1000000, raydir_y * 1000000};
	while (!hit)
	{
		player.x += step.x;
		player.y += step.y;
		printf("coord check: %d,%d\n", player.x / 1000000, player.y / 1000000);
		drawrectangle(img, (t_point){5, 5}, (t_point){player.x / 1000000 * 40 + 20, player.y / 1000000 * 40 + 20}, 0xFFF333FF);
		hit = lvl->map[player.y / 1000000][player.x / 1000000] == '1';
	}
	drawrectangle(img, (t_point){40, 40}, (t_point){player.x / 1000000 * 40, player.y / 1000000 * 40}, 0xFFFFFFFF);
	printf("Hit wall at %d,%d\n", player.x / 1000000, player.y / 1000000);
}
