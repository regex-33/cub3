#include "../inc/parsing.h"

void ft_exit(t_game *data) {
  mlx_destroy_window(data->mlx, data->win);
  free_garbage(data->garbage);
  exit(0);
}

int close_windows(void *param) {
  t_game *data = (t_game *)param;
  free_garbage(data->garbage);
  exit(0);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color) {
  char *dst;

  if (x < 0 || x >= S_W || y < 0 || y >= S_H)
    return;
  dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
  *(unsigned int *)dst = color;
}

void fill_screen(t_game *data) {
  int y;
  int x;

  for (y = 0; y < S_H; y++) {
    for (x = 0; x < S_W; x++) {
      my_mlx_pixel_put(&data->img, x, y, 0x212221);
    }
  }
}
bool is_wall(char c) { return (c == '1'); }

bool is_player(char c) {
  return (c == 'N' || c == 'S' || c == 'W' || c == 'E');
}
void put_player(t_game *data) {
  int i, j;
  int size = TILE_SIZE / 3; // player size

  i = -size / 2;
  while (i < size / 2) {
    j = -size / 2;
    while (j < size / 2)
      my_mlx_pixel_put(&data->img, data->player->plyr_x + i,
                       data->player->plyr_y + j++, 0x00FF0000);
    i++;
  }
}
void draw_map2d(t_game *data) {
  int i = 0;
  int j = 0;
  int x = 0;
  int y = 0;
  int size = TILE_SIZE * 0.43;
  while (i < data->map.h_map) {
    j = 0;
    while (j < data->map.w_map) {
      if (data->map.map[i][j] == '1') {
        x = j * size;
        y = i * size;
        while (x < (j + 1) * size - 1) {
          while (y < (i + 1) * size - 1) {
            my_mlx_pixel_put(&data->img, x, y, 0x00FFFFFF);
            y++;
          }
          y = i * size;
          x++;
        }
      }
      j++;
    }
    i++;
  }
}
float nor_angle(float angle) // normalize the angle
{
  if (angle < 0)
    angle += (2 * M_PI);
  if (angle > (2 * M_PI))
    angle -= (2 * M_PI);
  return (angle);
}

void draw_floor_ceiling(t_game *mlx, int ray, int t_pix,
                        int b_pix) // draw the floor and the ceiling
{
  int i;

  i = b_pix;
  while (i < S_H)
    my_mlx_pixel_put(
        &mlx->img, ray, i++,
        0xB99470); // floor
                   // my_mlx_pixel_put(mlx, ray, i++, 0xB99470FF); // floor
  i = 0;
  while (i < t_pix)
    my_mlx_pixel_put(
        &mlx->img, ray, i++,
        0x89CFF3); // ceiling
                   // my_mlx_pixel_put(mlx, ray, i++, 0x89CFF3FF); // ceiling
}

int get_color(t_game *mlx, int flag) // get the color of the wall
{
  mlx->ray->ray_ngl = nor_angle(mlx->ray->ray_ngl); // normalize the angle
  if (flag == 0) {
    if (mlx->ray->ray_ngl > M_PI / 2 &&
        mlx->ray->ray_ngl <
            3 * (M_PI / 2)) // check the angle if is not between 90 and 270
      return (0xB5B5B5);    // west wall
    else
      return (0xffff); // east wall // if the angle is between 90 and 270
  } else {
    if (mlx->ray->ray_ngl > 0 &&
        mlx->ray->ray_ngl < M_PI) // check the angle if is not between 0 and 180
      return (0xF5F5F5);          // south wall
    else
      return (0xffffff); // north wall // if the angle is between 0 and 180
  }
}

void draw_wall(t_game *mlx, int ray, int t_pix, int b_pix) // draw the wall
{
  int color;

  color = get_color(mlx, mlx->ray->flag);
  while (t_pix < b_pix)
    my_mlx_pixel_put(&mlx->img, ray, t_pix++, color);
  // my_mlx_pixel_put(mlx, ray, t_pix++, color);
}

void render_wall(t_game *mlx, int ray) // render the wall
{
  double wall_h;
  double b_pix;
  double t_pix;

  mlx->ray->distance *=
      cos(nor_angle(mlx->ray->ray_ngl - mlx->player->angle)); // fix the fisheye
  wall_h = (TILE_SIZE / mlx->ray->distance) *
           ((S_W / 2) / tan(mlx->player->fov_rd /
                            2)); // get the wall height // S_W / 2 = 950 we use
                                 // tan(30 / 2) = 0.57735026919
  b_pix = (S_H / 2) + (wall_h / 2); // get the bottom pixel
  t_pix = (S_H / 2) - (wall_h / 2); // get the top pixel
  if (b_pix > S_H)                  // check the bottom pixel
    b_pix = S_H;
  if (t_pix < 0) // check the top pixel
    t_pix = 0;
  draw_wall(mlx, ray, t_pix, b_pix);          // draw the wall
  draw_floor_ceiling(mlx, ray, t_pix, b_pix); // draw the floor and the ceiling
}
/*
int cast_rays(t_game *data)
{
    // printf("cast rays\n");
    // printf("Player position : %d, %d\n", data->map.p_x, data->map.p_y);
    // printf("Player position in pixels : %d, %d\n", data->player->plyr_x,
data->player->plyr_y);
    // printf("palyer pos: %d, %d\n", data->player->plyr_x +
data->player->delta_x * 5, data->player->plyr_y + data->player->delta_y * 5);
    fill_screen(data);
    draw_map2d(data);
    put_player(data);
    // for (int i = 0; i < 5; i++)
        // draw_line_dda(data, data->player->plyr_x, data->player->plyr_y,
data->player->plyr_x + data->player->delta_x * 5 + i, data->player->plyr_y +
data->player->delta_y * 5 + 1, 0x00FF0000);
    // drawRays2D(data);
    if (drawRays3D(data))
        return (1);

    // drawRays2D(&mlx_data, px, py, pa);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
    return 0;
}
*/

int update_player_position(t_game *data) {
  t_player *player = data->player;
  double move_step = player->walk_direction * player->walk_speed;
  double new_x = player->plyr_x + cos(player->angle) * move_step;
  double new_y = player->plyr_y + sin(player->angle) * move_step;

  if (player->turn_direction == -1) {
    // player->angle += 5;
    player->angle += player->turn_direction * player->turn_speed;
    if (player->angle < 0)
      player->angle += 2 * M_PI;
    if (player->angle > 2 * M_PI)
      player->angle -= 2 * M_PI;
    player->delta_x = cos(player->angle) * player->walk_speed;
    player->delta_y = -sin(player->angle) * player->walk_speed;
  }
  if (player->turn_direction == 1) {
    // player->angle -= 5;
    player->angle += player->turn_direction * player->turn_speed;
    if (player->angle < 0)
      player->angle += 2 * M_PI;
    if (player->angle > 2 * M_PI)
      player->angle -= 2 * M_PI;
    player->delta_x = cos(player->angle) * player->walk_speed;
    player->delta_y = -sin(player->angle) * player->walk_speed;
  }
  if (player->walk_direction == 1) {
    new_x = player->plyr_x + player->delta_x;
    new_y = player->plyr_y + player->delta_y;
  }
  if (player->walk_direction == -1) {
    new_x = player->plyr_x - player->delta_x;
    new_y = player->plyr_y - player->delta_y;
  }
  /*Check for wall collisions*/
  int map_x = (int)(new_x / 32) / 2;
  int map_y = (int)(new_y / 32) / 2;
  if (map_x < 0 || map_x >= data->map.w_map || map_y < 0 ||
      map_y >= data->map.h_map)
    return 0;
  if (data->map.map[map_y][map_x] != '1') {
    player->plyr_x = new_x;
    player->plyr_y = new_y;
  }
  return (cast_rays(data), 0);
}

int key_press(int keycode, t_game *data) {
  if (keycode == 65307) // ESC key
    return (free_garbage(data->garbage), exit(0), 1);
  if (keycode == 'w')
    data->player->walk_direction = 1;
  if (keycode == 's')
    data->player->walk_direction = -1;
  if (keycode == 'a')
    data->player->turn_direction = -1;
  if (keycode == 'd')
    data->player->turn_direction = 1;
  return (0);
}

int key_release(int keycode, t_game *data) {
  if (keycode == 'w' || keycode == 's')
    data->player->walk_direction = 0;
  if (keycode == 'a' || keycode == 'd')
    data->player->turn_direction = 0;
  return (0);
}

void game_loop(void *mlx) {
  t_game *data = (t_game *)mlx;
  data->win = mlx_new_window(data->mlx, S_W, S_H, "Cub3d");
  if (!data->win) {
    free_garbage(data->garbage);
    mlx_destroy_display(data->mlx);
    return (print_error("Error\nWindow creation failed\n", RED), exit(1));
  }
  data->img.img = mlx_new_image(data->mlx, S_W, S_H);
  data->img.addr = mlx_get_data_addr(data->img.img, &data->img.bits_per_pixel,
                                     &data->img.line_length, &data->img.endian);

  cast_rays(data);
  mlx_hook(data->win, 2, 1L << 0, key_press, data);
  mlx_hook(data->win, 3, 1L << 1, key_release, data);
  mlx_loop_hook(data->mlx, update_player_position, data);
  mlx_loop(data->mlx);
}

int main(int argc, char **argv) {
  t_game data;

  if (argc != 2)
    return (print_error("./Cub3d </path/map.cub>\n", YELLOW), 1);
  if (parsing(argv[1], &data))
    return (free_garbage(data.garbage), 1);

  printf("NO: %s\n", data.images->no);
  printf("SO: %s\n", data.images->so);
  printf("WE: %s\n", data.images->we);
  printf("EA: %s\n", data.images->ea);
  printf("F: %d\n", data.map.floor_color);
  printf("C: %d\n", data.map.ceiling_color);

  printf("Player position: %d, %d\n", data.map.p_x, data.map.p_y);

  printf("Player position in pixels: %d, %d\n", data.player->plyr_x,
         data.player->plyr_y);

  printf("Map size: %d, %d\n", data.map.w_map, data.map.h_map);

  printf("Felid of view: %f\n", data.player->fov_rd);
  printf("Rotation: %d\n", data.player->rot);
  printf("angle: %f\n", data.player->angle);
  printf("Map:\n");
  int i = 0;
  while (i < data.map.h_map) {
    printf("index: %d | line : %s\n", i, data.map.map[i]);
    i++;
  }
  game_loop(&data);
  free_garbage(data.garbage);

  return (0);
}
