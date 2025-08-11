/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:42:58 by nmattos-          #+#    #+#             */
/*   Updated: 2025/08/11 13:43:03 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

double calculate_perpendicular_distance(t_playerdata p, t_vect raydir, t_point map, int hit_side, t_point step)
{
	double	perp_wall_dist;

	if (hit_side == 0)
		perp_wall_dist = (map.x - p.x + (1 - step.x) / 2) / raydir.x;
	else
		perp_wall_dist = (map.y - p.y + (1 - step.y) / 2) / raydir.y;
	return (perp_wall_dist);
}

t_vect calculate_intersection(t_playerdata p, t_vect raydir, double perp_wall_dist)
{
	t_vect	intersect;

	intersect.x = p.x + raydir.x * perp_wall_dist;
	intersect.y = p.y + raydir.y * perp_wall_dist;
	return (intersect);
}
