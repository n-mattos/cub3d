/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_whitespaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:03:07 by nmattos-          #+#    #+#             */
/*   Updated: 2025/06/25 10:03:32 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

int	skip_whitespaces(char *str, int i)
{
	while (is_whitespace(str[i]))
		i++;
	return (i);
}
