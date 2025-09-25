/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:53:01 by nmattos-          #+#    #+#             */
/*   Updated: 2025/09/25 13:23:55 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <math.h>
# include <sys/time.h>
# include "MLX42/MLX42.h"
# include "../libft/libft.h"

# define USAGE "Please specify which map to use.\n./cub3D [dir/map.cub]\n"

/* Colors: [RGBA] */
# define BLACK 0x00000088
# define WHITE 0xFFFFFFFF
# define GRAY 0x808080FF
# define RED 0xFF0000FF

# define WALL_COLOR WHITE
# define EMPTY_COLOR BLACK
# define FLOOR_COLOR GRAY

# define LINESMOOTHNESS 1000

# define TURNSPEED 0.05		// radians
# define MOVESPEED 0.1
# define COLLISION_BUFFER 0.1

# define IMG_HEIGHT 1080.0
# define IMG_WIDTH 1920.0

# define MMAP_DIAM 400.0
# define MMAP_BORDER_THICKNESS 2
# define MMAP_SQUARE_BORDER 2

# define TOTAL_RAYS (IMG_WIDTH)		// total rays to cast (width of the screen)
# define PERCENTAGE_RAYS 10			// percentage of rays to display (minimap)

# define TEXTURE_WIDTH 64			// width of the texture (currently hardcoded to 64)
# define TEXTURE_HEIGHT 64			// height of the texture (currently hardcoded to 64)

# define SOURCE 0
# define TARGET 1

# define INVALID_POINT (t_point){-1, -1}

# define PI 3.14159265358979323846
# define HYPOTENUSE 1.41421356237 / 2
# define EPSILON 1.e-10

typedef enum e_tile {
	EMPTY = ' ',
	WALL = '1',
	FLOOR = '0',
	NORTH = 'N',
	EAST = 'E',
	SOUTH = 'S',
	WEST = 'W',
	DOOR = 'D',
	DOOR_OPEN = 'd',
	PORTAL = 'P'
}	t_tile;

typedef enum e_hit_side {
	NO_HIT = -1,
	VERTICAL = 0,
	HORIZONTAL = 1,
}	t_hit_side;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_vect
{
	double	x;
	double	y;
}	t_vect;

typedef struct s_playerdata {
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_playerdata;

typedef struct s_textures {
	mlx_texture_t	*north;
	mlx_texture_t	*east;
	mlx_texture_t	*south;
	mlx_texture_t	*west;
	mlx_texture_t	*portal[6];
	mlx_texture_t	*door;
	int				floor;
	int				ceiling;
}	t_textures;

typedef struct s_raycast {
	t_vect		raydir;
	t_vect		delta;
	t_vect		side;
	t_point		map;
	t_point		step;
	t_hit_side	hit_side;
	double		perp_wall_dist;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			txt_x;
	double		txt_pos;
	int			txt_y;
	int			tile;
	int			frame;
}	t_raycast;

typedef struct s_portal_list {
	char					id;
	t_point					A[2];
	t_point					B[2];
	struct s_portal_list	*next;
}	t_portal_list;

typedef struct s_door_list {
	t_point				pos;
	bool				is_open;
	struct s_door_list	*next;
}	t_door_list;

typedef struct s_gif
{
	double	current;
	double	last;
	int		frame;
}	t_gif;

typedef struct s_level {
	int				**map;
	t_portal_list	*portals;
	t_door_list		*doors;
	t_textures		*textures;
	t_playerdata	*player;
}	t_level;

typedef struct s_data {
	mlx_t		*mlx;
	mlx_image_t	*minimap;
	mlx_image_t	*last_frame;
	mlx_image_t	*background;
	t_level		*level;
	t_gif		*gif;
	int			rect;
	double		delta_time;
	double		move_speed;
	double		turn_speed;
	double		prev_mouse_x;
}	t_data;

/*****************************************************************************\
*	Function Prototypes														  *
\*****************************************************************************/

/* parse */
t_level		*parse(char *fn_map);
t_textures	*parse_textures(int fd);
t_level		*parse_map(int fd);
bool		map_is_valid(int **map);
bool		get_portals(int **map, t_portal_list **portals);

/* parse_memory */
void		free_raw_textures(char **raw_textures);
void		free_textures(t_textures *textures);
void		free_map(int **map, int i);
void		free_level(t_level *level);
t_textures	*allocate_textures(void);

/* player */
t_playerdata	*retrieve_player(int **map);
int				player_starting_direction(t_playerdata *player, int direction);

/* movement */
void	player_input(t_data *d);
void	keys(mlx_key_data_t keydata, void *data);
void	mouse_move(double x, double y, void *data);

/* raycast */
void	raycast_dda(t_data *d);

/* calculations */
t_vect	calculate_raydir(mlx_image_t *img, t_playerdata p, int x);
t_vect	calculate_side(t_playerdata p, t_raycast *ray, t_point map);
t_vect	calculate_delta(t_vect raydir);
t_point	calculate_map(t_playerdata p);
void	calculate_ray(t_point *map, t_raycast *ray, t_level *lvl);
double	calculate_perpendicular_distance(t_playerdata p, t_raycast *ray, t_point map);
t_vect	calculate_intersection(t_playerdata p, t_vect raydir, double perp_wall_dist);
double	calculate_wallx(t_playerdata *p, int hit_side, double perp_wall_dist, t_vect raydir);

/* draw */
void	draw_all(t_data *d);
void	draw_circle_outline(mlx_image_t *img, t_point center, int radius, uint32_t color);
void	fill_circle(mlx_image_t *img, t_point center, int radius, uint32_t color);
void	drawvert(mlx_image_t *img, t_point a, t_point b, uint32_t color);
void	draw_textured_wall(t_raycast *ray, t_data *d, int x);
void	draw_wall(mlx_image_t *img, double perp_dist, int side, int x);
void	drawline(mlx_image_t *img, t_point a, t_point b, uint32_t color);
void	drawrectangle(mlx_image_t *img, t_point wh, t_point coord, uint32_t color);
uint32_t	get_pixel_color(t_textures *textures, t_raycast *ray);

/* draw/minimap */
void	draw_minimap(t_data *d);
void	draw_minimap_rays(t_data *d, t_playerdata p, t_vect intersect, int x);

/* utils */
size_t	chars_till_eol(char *str);
bool	is_whitespace(char c);
int		skip_whitespaces(char *str, int i);
size_t	count_char(char *str, char c);
bool	is_player(int c);

/* portal list */
t_portal_list	*create_portal_node(char id, t_point sourceB, t_point targetA);
t_portal_list	*append_portal_node(t_portal_list **head, t_portal_list *new_node);
t_portal_list	*find_portal_node(t_portal_list *head, char id);
void			update_portal_node(t_portal_list *node, t_point sourceB, t_point targetA);
t_portal_list	*free_portal_list(t_portal_list **head);

/* door list */
t_door_list	*create_door_node(t_point position, bool is_open);
t_door_list	*append_door_node(t_door_list **head, t_door_list *new_node);
t_door_list	*find_door_node(t_door_list *head, t_point pos);
t_door_list	*free_door_list(t_door_list **head);

#endif
