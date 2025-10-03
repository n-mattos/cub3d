/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattos- <nmattos-@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:53:01 by nmattos-          #+#    #+#             */
/*   Updated: 2025/10/03 12:17:32 by nmattos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
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

# define IMG_HEIGHT 1080
# define IMG_WIDTH 1920

# define MMAP_DIAM 400.0
# define MMAP_BORDER_THICKNESS 2
# define MMAP_SQUARE_BORDER 2

# define TOTAL_RAYS IMG_WIDTH		// total rays to cast (width of the screen)
# define PERCENTAGE_RAYS 10			// percentage of rays to display (minimap)

# define TEXTURE_WIDTH 64			// width of the texture
# define TEXTURE_HEIGHT 64			// height of the texture

# define SOURCE 0
# define TARGET 1

# define PI 3.14159265358979323846
# define HYPOTENUSE 0.70710678118654752440 // sqrt(2)/2
# define EPSILON 1.e-10

typedef enum e_tile
{
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

typedef enum e_hit_side
{
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

typedef struct s_playerdata
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_playerdata;

typedef struct s_textures
{
	mlx_texture_t	*north;
	mlx_texture_t	*east;
	mlx_texture_t	*south;
	mlx_texture_t	*west;
	mlx_texture_t	*portal[6];
	mlx_texture_t	*door[21];
	int				floor;
	int				ceiling;
}	t_textures;

typedef struct s_raycast
{
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

typedef struct s_portal_list
{
	char					id;
	t_point					a[2];
	t_point					b[2];
	struct s_portal_list	*next;
}	t_portal_list;

typedef enum e_door_state
{
	CLOSED = 0,
	OPENING = 1,
	OPEN = 2,
	CLOSING = 3
}	t_door_state;

typedef struct s_door_list {
	t_point				pos;
	t_door_state		state;
	mlx_texture_t		*texture;
	int					index;
	struct s_door_list	*next;
}	t_door_list;

typedef struct s_gif
{
	double	current;
	double	last;
	int		frame;
}	t_gif;

typedef struct s_level
{
	int				**map;
	int				portal_effect_opacity;
	t_portal_list	*portals;
	t_door_list		*doors;
	t_textures		*textures;
	t_playerdata	*player;
}	t_level;

typedef struct s_data
{
	mlx_t		*mlx;
	mlx_image_t	*minimap;
	mlx_image_t	*last_frame;
	mlx_image_t	*background;
	mlx_image_t	*portal_effect;
	mlx_image_t	*crosshair;
	t_level		*level;
	t_gif		*gif_portal;
	int			rect;
	double		delta_time;
	double		move_speed;
	double		turn_speed;
	double		prev_mouse_x;
}	t_data;

/*	Function Prototypes
\************************************************************************/

/* parse */
t_level			*parse(char *fn_map);
t_textures		*parse_textures(int fd);
t_textures		*sort_texture_data(char **raw, t_textures *textures);
t_level			*parse_map(int fd);
bool			map_is_valid(int **map);
bool			get_portals(int **map, t_portal_list **portals);
bool			get_doors(int **map, t_door_list **doors);

/* parse_memory */
void			free_raw_textures(char **raw_textures);
void			free_textures(t_textures *textures);
void			free_map(int **map, int i);
void			free_level(t_level *level);
t_textures		*allocate_textures(void);

/* game loop */
void			run_game(mlx_t *mlx, t_data *data);

/* player */
t_playerdata	*retrieve_player(int **map);
int				player_starting_direction(t_playerdata *player, int direction);

/* player_input */
void			player_input(t_data *d);
void			single_press_interactions(mlx_key_data_t keydata, void *data);
void			mouse_move(double x, double y, void *d);
void			turn_keys(t_data *d, t_playerdata *p);
void			move_player(t_data *d, t_playerdata *p);
void			door_interaction(t_data *d, mlx_key_data_t keydata);
void			collision(t_level *level, t_vect new);

/* raycast */
void			raycast_dda(t_data *d);
t_raycast		single_ray(t_data *d, t_playerdata p, int x);

/* calculations */
t_vect			calculate_raydir(mlx_image_t *img, t_playerdata p, int x);
t_vect			calculate_side(t_playerdata p, t_raycast *ray, t_point map);
t_vect			calculate_delta(t_vect raydir);
t_point			calculate_map(t_playerdata p);
void			calculate_ray(t_point *map, t_raycast *ray,
					t_level *lvl, bool check_door);
double			calculate_perpendicular_distance(t_playerdata p,
					t_raycast *ray, t_point map);
t_vect			calculate_intersection(t_playerdata p, t_vect raydir,
					double perp_wall_dist);
double			calculate_wallx(t_playerdata *p, int hit_side,
					double perp_wall_dist, t_vect raydir);

/* draw */
void			draw_all(t_data *d);
void			draw_circle_outline(mlx_image_t *img, t_point center,
					int radius, uint32_t color);
void			fill_circle(mlx_image_t *img, t_point center, int radius,
					uint32_t color);
bool			in_circle(t_point point, t_point center, int radius);
void			drawvert(mlx_image_t *img, t_point a, t_point b,
					uint32_t color);
void			draw_textured_wall(t_raycast *ray, t_data *d, int x);
void			draw_door(t_raycast *ray, t_data *d, int x);
void			draw_wall(mlx_image_t *img, double perp_dist,
					int side, int x);
void			drawline(mlx_image_t *img, t_point a, t_point b,
					uint32_t color);
void			drawrectangle(mlx_image_t *img, t_point wh, t_point coord,
					uint32_t color);
uint32_t		get_pixel_color(t_level *level, t_raycast *ray);
void			draw_minimap(t_data *d);
void			draw_minimap_rays(t_data *d, t_playerdata p,
					t_vect intersect, int x);
void			draw_floor_ceiling(mlx_image_t *img, t_textures *textures);
void			create_background(t_data *d);
void			create_last_frame(t_data *d);
void			create_minimap(t_data *d);
void			create_portal_effect(t_data *d);
void			create_crosshair(t_data *d);

/* utils */
size_t			chars_till_eol(char *str);
bool			is_whitespace(char c);
int				skip_whitespaces(char *str, int i);
size_t			count_char(char *str, char c);
bool			is_player(int c);

/* portal list */
t_portal_list	*create_portal_node(char id, t_point sourceB, t_point targetA);
t_portal_list	*append_portal_node(t_portal_list **head,
					t_portal_list *new_node);
t_portal_list	*find_portal_node(t_portal_list *head, char id);
void			update_portal_node(t_portal_list *node,
					t_point sourceB, t_point targetA);
t_portal_list	*free_portal_list(t_portal_list **head);

/* door list */
t_door_list		*create_door_node(t_point position);
t_door_list		*append_door_node(t_door_list **head, t_door_list *new_node);
t_door_list		*find_door_node(t_door_list *head, t_point pos);
t_door_list		*free_door_list(t_door_list **head);
mlx_texture_t	*get_door_texture(t_door_list *door, t_point door_pos);
void			update_doors(t_door_list *doors, t_level *level, double delta_time);
void			trigger_door(t_level *level, t_point door_pos);

#endif
