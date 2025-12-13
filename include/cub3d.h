/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 12:53:01 by nmattos-          #+#    #+#             */
/*   Updated: 2025/12/13 17:37:25 by mschippe         ###   ########.fr       */
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
# include <limits.h>
# include "MLX42/MLX42.h"
# include "../libft/libft.h"

# define USAGE \
"Please specify which map to use.\n./cub3D [dir/map.cub]\n"
# define MSG_COMBINE_FAIL \
"Error\nMap combining failed\n"
# define MSG_ERR_BUT_SUCCESS \
"Error\nParsing was successful but error still thrown\n"
# define MSG_ERR_TEX_LOAD_FAIL \
"Error\nTexture(s) missing from file or failed to load\n"
# define MSG_ERR_WRONG_COLOR \
"Error\nFloor or ceiling is formatted incorrectly\n"
# define MSG_ERR_MALLOC_FAIL \
"Error\nA memory allocation failed during parsing\n"
# define MSG_ERR_REDEFINE \
"Error\nAt least one field in the file was redefined\n"
# define MSG_ERR_COLOR_MISSING \
"Error\nAt least one color is missing from the file\n"
# define MSG_ERR_PARSE_FAIL \
"Error\nSomething went wrong in map parsing\n"
# define MSG_ERR_MAP_INVALID \
"Error\nMap is invalid\n"
# define MSG_ERR_PORTALS_INVALID \
"Error\nMap contains invalid portals (could be placement or amount related)\n"
# define MSG_ERR_DOORS_INVALID \
"Error\nInvalid doors (could be placement or amount related)\n"
# define MSG_ERR_NO_PLAYER \
"Error\nNo player found\n"
# define MSG_ERR_TEX_PARSE_FAIL \
"Error\nFailed to parse textures\n"
# define MSG_ERR_STATIC_TEX_FAIL \
"Error\nFailed to load static textures\n"
# define MSG_ERR_NOT_CUB \
"Error\nFile is not a .cub file\n"
# define MSG_ERR_SIZE_CALC_FAIL \
"Error\nFailed to open .cub file during map size calculation\n"
# define MSG_ERR_FILE_OPEN_FAIL \
"Error\nFailed to open .cub file\n"
# define MSG_ERR_FILE_TOO_SMALL \
"Error\nFile too small to be valid\n"
#define MSG_ERR_FILE_READ_MALLOC_FAIL \
"Error\nMemory allocation failed for file read\n"
# define MSG_ERR_FILE_SIZE_MISMATCH \
"Error\nFound more lines than expected\n"
# define MSG_ERR_MLX_INIT_FAIL \
"Error\nFailed to initialize mlx\n"
# define MSG_ERR_DATA_ALLOC_FAIL \
"Error\nFailed to allocate memory for game data\n"
# define MSG_ERR_ANIM_ALLOC_FAIL \
"Error\nFailed to allocate memory for gif data\n"

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
# define COLLISION 0.15

# define IMG_HEIGHT 1080
# define IMG_WIDTH 1920

# define MMAP_DIAM 400.0
# define MMAP_BORDER_THICKNESS 2
# define MMAP_SQUARE_BORDER 2

# define TOTAL_RAYS IMG_WIDTH		// total rays to cast (width of the screen)
# define PERCENTAGE_RAYS 10			// percentage of rays to display (minimap)

# define TEXTURE_WIDTH 64			// width of the texture
# define TEXTURE_HEIGHT 64			// height of the texture

# define PORTAL_FRAME_COUNT 6		// the amount of frames in the portal animation
# define DOOR_FRAME_COUNT 21		// the amount of frames in the door animation

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

typedef enum e_tex_info_type
{
	TI_NONE,
	TI_NORTH,
	TI_EAST,
	TI_SOUTH,
	TI_WEST,
	TI_FLOOR,
	TI_CEILING
}	t_tex_info_type;

typedef enum e_parse_tex_res
{
	TIS_SUCCESS,
	TIS_TEXTURE_LOAD_FAIL,
	TIS_INVALID_COLOR_FORMAT,
	TIS_MALLOC_FAIL,
	TIS_REDEFINE,
	TIS_COLOR_MISSING,
}	t_parse_tex_res;

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

typedef struct s_tex_redef_check
{
	int	north;
	int	east;
	int	south;
	int	west;
	int	floor;
	int	ceiling;
}	t_tex_redef_check;

typedef struct s_textures
{
	int					tex_line_offset;
	t_tex_redef_check	redef_check;
	mlx_texture_t	*north;
	mlx_texture_t	*east;
	mlx_texture_t	*south;
	mlx_texture_t	*west;
	mlx_texture_t	*portal[PORTAL_FRAME_COUNT];
	mlx_texture_t	*door[DOOR_FRAME_COUNT];
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
	bool		transparent;
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
	bool		mouse_enabled;
}	t_data;

/*	Function Prototypes
\************************************************************************/

void			exit_program(mlx_t *mlx, t_data *data);

/* parse */
t_level			*parse(char *fn_map);
t_textures		*parse_textures(int fd);
t_textures		*sort_texture_data(char **raw, t_textures *textures);
t_level			*parse_map(char *map_str);
bool			map_is_valid(int **map);
bool			get_portals(int **map, t_portal_list **portals);
bool			get_doors(int **map, t_door_list **doors);

/* parse_memory */
void			free_raw_textures(char **raw_textures);
void			free_textures(t_textures *textures);
void			free_map(int **map, int i);
void			free_level(t_level *level);
t_textures		*allocate_textures(void);
void			free_2d_array(char **array);

/* new parser */
bool			cub_strcmp(char *s1, char *s2);
bool			is_cub_file(char *fn);
char			**read_cub_file(char *fn);
t_textures		*new_parse_textures(char **lines);
t_parse_tex_res	validate_parsed_textures(t_textures *tex);
char			*join_map_lines(t_textures *tex, char **lines);
bool			update_tex_defined(t_tex_info_type type,
					t_tex_redef_check *redef);
bool			load_door_tex(t_textures *tex);
bool			load_portal_tex(t_textures *tex);
void			free_array(void **arr, void (*del)(void *));
int				get_line_count(char *fn);
bool			read_into_array(char **arr, int fd, int size);
size_t			num_spaces(char *str);
bool			is_info_line(char *line);
bool			is_map_char(char c);
bool			is_map_line(char *line);
t_tex_info_type	get_tex_type(char *line);
char			*get_tex_value(char *line);
bool			rgb_atoi(const char *nptr, int *target);
size_t			ft_arrlen(void **arr);
int				string_to_rgb(char *value);
void			insert_tex(t_textures *tex, t_tex_info_type type, char *value);
char			*create_filename(char *pre, int num, char *post);
bool			load_portal_tex(t_textures *tex);
bool			load_door_tex(t_textures *tex);
t_parse_tex_res	is_redef_err(t_parse_tex_res res, t_tex_redef_check *redef);

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
void			draw_textured_wall(t_raycast *ray, t_data *d, int x, bool draw_door);
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
void			update_doors(t_door_list *doors, t_level *level);
void			trigger_door(t_level *level, t_point door_pos);

#endif
