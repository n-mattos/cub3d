NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -O2 -march=native -ffast-math
# CFLAGS = -g

# Library paths
MLX42_PATH = ./include/MLX42
LIBFT_PATH = ./include/libft
MLX42_REPO = https://github.com/codam-coding-college/MLX42.git
MLX42_VERSION = ce254c3
LIBFT_REPO = https://github.com/put/libft.git
LIBFT_VERSION = e99f0e6

# Headers and libraries
HEADERS = -I$(MLX42_PATH)/include -I$(LIBFT_PATH)
LIBS = $(MLX42_PATH)/build/libmlx42.a $(LIBFT_PATH)/libft.a -ldl -lglfw -pthread -lm

SRCS =	src/main.c \
		src/game.c \
		src/parsing/parse.c \
		src/parsing/parse_map.c \
		src/parsing/parse_textures.c \
		src/parsing/load_textures.c \
		src/parsing/map_validator.c \
		src/parsing/parse_memory.c \
		src/parsing/new_parser.c \
		src/utils/chars_till_eol.c \
		src/utils/count_char.c \
		src/utils/is_player.c \
		src/utils/is_whitespace.c \
		src/utils/skip_whitespaces.c \
		src/raycasting/raycast.c \
		src/raycasting/calc_1.c \
		src/raycasting/calc_2.c \
		src/draw/minimap.c \
		src/draw/draw_utils.c \
		src/draw/draw.c \
		src/draw/draw_texture.c \
		src/draw/draw_shapes.c \
		src/draw/images.c \
		src/player_input/input.c \
		src/player_input/door.c \
		src/player_input/collision.c \
		src/player_input/move.c \
		src/player_input/turn.c \
		src/player/player.c \
		src/list/portals.c \
		src/list/doors_list.c \
		src/list/doors_animation.c

OBJDIR = obj
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

all: mlx42 libft $(NAME)

mlx42: $(MLX42_PATH)/build/libmlx42.a

$(MLX42_PATH)/build/libmlx42.a:
	@if [ ! -d "$(MLX42_PATH)" ]; then \
		git clone $(MLX42_REPO) $(MLX42_PATH); \
		cd $(MLX42_PATH) && git checkout $(MLX42_VERSION); \
	fi
	@mkdir -p $(MLX42_PATH)/build
	@cd $(MLX42_PATH) && cmake -B build && cmake --build build -j4

libft: $(LIBFT_PATH)/libft.a

$(LIBFT_PATH)/libft.a:
	@if [ ! -d "$(LIBFT_PATH)" ]; then \
		git clone $(LIBFT_REPO) $(LIBFT_PATH); \
		cd $(LIBFT_PATH) && git checkout $(LIBFT_VERSION); \
	fi
	@make all -C $(LIBFT_PATH)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

clean:
	@make -C $(LIBFT_PATH) clean
	@rm -rf $(OBJDIR)
	@rm -rf $(MLX42_PATH)/build

fclean: clean
	@make -C $(LIBFT_PATH) fclean
	@rm -f $(NAME)
	@rm -rf $(MLX42_PATH)
	@rm -rf $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re mlx42 libft