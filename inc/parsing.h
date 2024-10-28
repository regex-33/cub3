#ifndef CUB3D_H
#define CUB3D_H

#define MINIMAP_SCALE 0.2
#define MINIMAP_TILE_SIZE (TILE_SIZE * MINIMAP_SCALE)
#define MINIMAP_PLAYER_SIZE 3
#define MINIMAP_COLOR_WALL 0xFFFFFF
#define MINIMAP_COLOR_SPACE 0x000000
#define MINIMAP_COLOR_PLAYER 0xFF0000

#include "../lib/libft/includes/ft_printf.h"
#include "../lib/libft/includes/get_next_line.h"
#include "../lib/libft/includes/libft.h"

#include "../lib/libft/includes/libft.h"
#include "../lib/mlx_linux/mlx.h"
#include <X11/keysym.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

#define ERROR "\033[0;31mError\n\033[0m"
#define PROGRAM_NAME "Cub3d"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// # include "MLX/MLX42.h" // include the mlx library

#define DR 0.0174533 // 1 degree in radian
#define S_W 1200
#define S_H 800
#define TILE_SIZE 32
#define FOV 60
#define ROTATION_SPEED 0.045
#define PLAYER_SPEED 4

typedef struct s_garbage {
  char *line;
  struct s_garbage *next;
} t_garbage;

typedef struct s_images {
  char *no;
  char *so;
  char *we;
  char *ea;
} t_images;

typedef struct s_map {
  char **map;
  int w_map;
  int h_map;
  int p_x; // player x position in the map
  int p_y; // player y position in the map
  int floor_color;
  int ceiling_color;
  int index;
} t_map;

typedef struct s_img {
  void *img;
  char *addr;
  int bits_per_pixel;
  int line_length;
  int endian;
} t_img;

typedef struct s_player {
  char direction;
  int plyr_x; // player x position in pixels
  int plyr_y; // player y position in pixels

  double angle; // player angle
  float fov_rd; // field of view in radians
  int rot;      // rotation flag
  int l_r;      // left right flag
  int u_d;      // up down flag

  int delta_x;
  int delta_y;

  double turn_direction;
  double walk_direction;
  double walk_speed;
  double turn_speed;
} t_player;

typedef struct s_game {
  void *mlx;
  void *win;

  int pxl_x;
  int pxl_y;

  t_map map;
  t_img img;
  t_player *player;
  t_images *images;
  t_garbage *garbage;

} t_game;

// parsing.c
int parsing(char *path, t_game *data);
int store_and_parse_map(char *line, t_game *data);
int count_map_lines(char *path);
int check_images_exist(t_game *data);
int check_take_imageName(char *line, t_game *data);
int set_image(t_game *data, char **current_image, char *error_message,
              char **split);
int have_player(char *line, t_game *data);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);

// main.c

// utils.c
int ft_cout_lines_begin_with(char *path, char c);
int map_extention(char *path);
void ft_replace(char *line, char s, char r);
int number_of_words(char **str);
int map_line_is_valid(char *line);
void print_error(char *error, char *color);

// init.c

int init_arguments(char *path, t_game *data);

// free_memory.c

int add_to_garbage(t_garbage **garbage, void *line);
void free_garbage(t_garbage *garbage);
void free2d(char **str);

// parse_color.c

int validate_color(t_game *data, char *color_str);
int extract_colors(char *line, t_game *data);
int process_color(t_game *data, char *color_str, int *color_dest,
                  char *error_msg);

#endif
