#include "../inc/parsing.h"

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
  free_garbage(data.garbage);

  return (0);
}
