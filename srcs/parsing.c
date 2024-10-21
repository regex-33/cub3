#include "../inc/parsing.h"
#include <stdio.h>

int player_around_walls(t_game *data) {
  char *line;
  int i;

  i = data->map.p_x;
  line = data->map.map[data->map.p_y];
  int down = data->map.p_y + 1;
  int up = data->map.p_y - 1;
  int left = data->map.p_x - 1;
  int right = data->map.p_x + 1;

  if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W' || line[i] == 'E') {
    if (up < 0 || down >= data->map.h_map || left < 0 || right < 0 ||
        left >= (int)ft_strlen(data->map.map[up]) ||
        right >= (int)ft_strlen(data->map.map[up]) ||
        left >= (int)ft_strlen(data->map.map[down]) ||
        right >= (int)ft_strlen(data->map.map[down])) {
      return (0);
    }
    if (line[left] == '1' && line[right] == '1' &&
        data->map.map[up][i] == '1' && data->map.map[down][i] == '1' &&
        data->map.map[up][left] == '1' && data->map.map[up][right] == '1' &&
        data->map.map[down][left] == '1' && data->map.map[down][right] == '1') {
      return (1);
    }
  }
  return (0);
}

bool isZeroSurrounded(t_game *game, int row, int col, int rows, int cols) {
  if (row > 0 && (game->map.map[row - 1][col] != '1' &&
                  game->map.map[row - 1][col] != '0' &&
                  game->map.map[row - 1][col] != game->player->direction)) {
    return false; // Top cell is not valid
  }
  if (row < rows - 1 &&
      (game->map.map[row + 1][col] != '1' &&
       game->map.map[row + 1][col] != '0' &&
       game->map.map[row + 1][col] != game->player->direction)) {
    return false; // Bottom cell is not valid
  }
  if (col > 0 && (game->map.map[row][col - 1] != '1' &&
                  game->map.map[row][col - 1] != '0' &&
                  game->map.map[row][col - 1] != game->player->direction)) {
    return false; // Left cell is not valid
  }
  if (col < cols - 1 &&
      (game->map.map[row][col + 1] != '1' &&
       game->map.map[row][col + 1] != '0' &&
       game->map.map[row][col + 1] != game->player->direction)) {
    return false;
  }
  return true;
}

int cheack_walls(t_game *game) {
  int cols = 0;
  // int y = 0;
  for (int y = 0; y < game->map.index; y++) {
    cols = ft_strlen(game->map.map[y]);
    for (int x = 0; x < cols; x++) {
      printf("x : %d && y : %d\n", x, y);
      if (game->map.map[y][x] == '0' ||
          game->map.map[y][x] == game->player->direction) {
        if (!isZeroSurrounded(game, y, x, game->map.index, cols)) {
          printf("Invalid map! '0' at position (%d, %d) is not properly "
                 "surrounded.\n",
                 y, x);
          return (false);
        }
      }
    }
    cols = 0;
  }
  return true;
}

int have_player(char *line, t_game *data) {
  int i;

  i = 0;
  while (line[i] != '\0') {
    if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W' || line[i] == 'E') {
      if (data->player->direction)
        return (print_error("Player already exists\n", RED), 1);
      data->player->direction = line[i];
      data->map.p_x = i;
      data->map.p_y = data->map.index;
    } else if (line[i] != '1' && line[i] != '0' && line[i] != ' ')
      return (print_error("Mapp is not Valid\n", RED), 1);
    i++;
  }
  return (0);
}

int set_image(t_game *data, char **current_image, char *error_message,
              char **split) {
  int count;
  char *new_image_path;

  // case : <spaces>no path<sapces>
  new_image_path = split[1];
  count = number_of_words(split);
  printf("number count : %d\n", count);
  if (count != 2)
    return (print_error("Image path is not valid\n", RED), free2d(split),
            free_garbage(data->garbage), exit(1), 1);
  if (*current_image)
    return (print_error(error_message, RED), free2d(split),
            free_garbage(data->garbage), exit(1), 1);
  *current_image = ft_strdup(new_image_path);
  if (!current_image)
    return (perror(PROGRAM_NAME), free2d(split), 1);
  add_to_garbage(&data->garbage, *current_image);
  if (!*current_image)
    return (perror(PROGRAM_NAME), free2d(split), 1);
  return (free2d(split), 0);
}

int check_take_imageName(char *line, t_game *data) {
  char **split;

  if (check_images_exist(data))
    return (1);
  split = ft_split(line, ' ');
  if (!split)
    return (perror(PROGRAM_NAME), 1);
  if (!ft_strcmp(split[0], "NO"))
    return set_image(data, &data->images->no, "NO image already exist\n",
                     split);
  else if (!ft_strcmp(split[0], "SO"))
    return set_image(data, &data->images->so, "SO image already exist\n",
                     split);
  else if (!ft_strcmp(split[0], "WE"))
    return set_image(data, &data->images->we, "WE image already exist\n",
                     split);
  else if (!ft_strcmp(split[0], "EA"))
    return set_image(data, &data->images->ea, "EA image already exist\n",
                     split);
  return (free2d(split), 1);
}

int check_images_exist(t_game *data) {
  if (data->images->no && data->images->so && data->images->we &&
      data->images->ea)
    return (1);
  return (0);
}

int count_map_lines(char *path) {
  int fd;
  char *line;
  int map_lines;
  int begin_one;

  map_lines = 0;
  begin_one = ft_cout_lines_begin_with(path, '1');
  if (map_extention(path))
    return (print_error("Map file is not .cub\n", RED), -1);
  fd = open(path, O_RDONLY);
  if (fd == -1)
    return (print_error("Can't open file\n", RED), -1);
  line = get_next_line(fd);
  while (line) {
    ft_replace(line, ' ', '1');
    if (!map_lines && line[0] == '\n') {
      free(line);
      line = get_next_line(fd);
      continue;
    }
    if (line[0] == '1')
      map_lines++;
    else if ((map_lines && line[0] != '1' && line[0] != '\n') ||
             (line[0] == '\n' && map_lines != begin_one))
      return (close(fd), free(line), print_error("Map is not vValid\n", RED),
              -1);
    free(line);
    line = get_next_line(fd);
  }
  if (!map_lines)
    return (close(fd), print_error("Map is empty\n", RED), -1);
  return (close(fd), map_lines);
}

int store_and_parse_map(char *line, t_game *data) {
  int i;
  char *temp;
  int len = 0;

  i = 0;
  // d'not forget to remove spaces form begin of the line and the end of line.
  ft_replace(line, '\t', ' ');
  temp = ft_strtrim(line, " \n");
  if (!temp)
    return (perror(PROGRAM_NAME), 1);
  free(line);
  line = temp;
  len = ft_strlen(line);
  if (len > data->map.w_map)
    data->map.w_map = len;
  // if (line[ft_strlen(line) - 1] == '\n')
  //   line[ft_strlen(line) - 1] = '\0';
  if (!data->map.index || data->map.index == data->map.h_map - 1) {
    while (line[i] != '\0') {
      if (line[i] != '1' && line[i] != ' ')
        return (print_error("Map is Nnot closed by 1\n", RED), 1);
      i++;
    }
  } else if (line[0] == '1' && line[ft_strlen(line) - 1] == '1') {
    if (have_player(line, data))
      return (1);
  } else
    return (print_error("nMap is not Valid\n", RED), 1);
  data->map.map[data->map.index] = ft_strdup(line);
  if (!data->map.map[data->map.index])
    return (perror(PROGRAM_NAME), 1);
  add_to_garbage(&data->garbage, data->map.map[data->map.index]);
  data->map.index++;
  data->map.map[data->map.index] = NULL;
  return (0);
}

void init_the_player(t_game *data) {
  data->player->plyr_x = data->map.p_x * TILE_SIZE + TILE_SIZE / 2;
  data->player->plyr_y = data->map.p_y * TILE_SIZE + TILE_SIZE / 2;

  data->player->fov_rd = (FOV * M_PI) / 180;
  data->player->angle = M_PI;

  data->player->turn_direction = 0;
  data->player->walk_direction = 0;
  data->player->walk_speed = 3;
  data->player->turn_speed = 1 * (M_PI / 180);

  data->player->delta_x = cos(data->player->angle) * data->player->walk_speed;
  data->player->delta_y = sin(data->player->angle) * data->player->walk_speed;
}

int init_map(t_game *data, char *path) {
  int i;

  i = 0;
  data->map.floor_color = 0;
  data->map.ceiling_color = 0;
  data->map.map = NULL;
  data->map.w_map = 0;
  data->map.h_map = count_map_lines(path);
  if (data->map.h_map <= 0)
    return (1);
  data->map.map = ft_calloc(data->map.h_map + 1, sizeof(char *));
  if (!data->map.map)
    return (perror(PROGRAM_NAME), 1);
  add_to_garbage(&data->garbage, data->map.map);
  while (i < data->map.h_map)
    data->map.map[i++] = NULL;
  data->map.map[data->map.h_map] = NULL;
  data->map.index = 0;
  return 0;
}

int init_arguments(char *path, t_game *data) {
  // int i = 0;
  data->garbage = NULL;

  /* don't forget to free memory */
  data->mlx = mlx_init();
  if (!data->mlx)
    return (print_error("Mlx failed\n", RED), 1);
  data->imposter.img_back = mlx_xpm_file_to_image(
      data->mlx, "img/test.xpm", &data->imposter.w, &data->imposter.h);
  if (!data->imposter.img_back)
    return (print_error("Imposter images not found back\n", RED), 1);
  data->imposter.img_front = mlx_xpm_file_to_image(
      data->mlx, "img/front.xpm", &data->imposter.w, &data->imposter.h);
  if (!data->imposter.img_front)
    return (print_error("Imposter images not found front\n", RED), 1);
  data->imposter.img_right = mlx_xpm_file_to_image(
      data->mlx, "img/right.xpm", &data->imposter.w, &data->imposter.h);
  if (!data->imposter.img_right)
    return (print_error("Imposter images not found right\n", RED), 1);
  data->imposter.img_left = mlx_xpm_file_to_image(
      data->mlx, "img/left.xpm", &data->imposter.w, &data->imposter.h);
  if (!data->imposter.img_left)
    return (print_error("Imposter images not found left\n", RED), 1);

  data->images = ft_calloc(1, sizeof(t_images));
  add_to_garbage(&data->garbage, data->images);

  if (init_map(data, path))
    return 1;
  data->player = ft_calloc(1, sizeof(t_player));
  if (!data->player)
    return (perror(PROGRAM_NAME), 1);
  add_to_garbage(&data->garbage, data->player);
  data->player->direction = 0;

  data->ray = ft_calloc(1, sizeof(t_ray));
  if (!data->ray)
    return (perror(PROGRAM_NAME), 1);
  add_to_garbage(&data->garbage, data->ray);

  return (0);
}

int have_images(t_game *data) {
  if (!data->images->no || !data->images->so || !data->images->we ||
      !data->images->ea)
    return (print_error("Missing image\n", RED), 0);
  return (1);
}

int parsing(char *path, t_game *data) {
  int fd;
  char *line;
  int flag;

  data->map.index = 0;
  if (init_arguments(path, data))
    return (1);
  fd = open(path, O_RDONLY);
  if (fd == -1)
    return (print_error("Can't open file\n", RED), 1);
  line = get_next_line(fd);
  while (line) {
    if (line[0] == '\n' || !check_take_imageName(line, data)) {
      free(line);
      line = get_next_line(fd);
      continue;
    }
    flag = extract_colors(line, data);
    if (!flag)
      return (close(fd), free(line), 1);
    else if (flag == 1) {
      if (!have_images(data))
        return (close(fd), free(line), 1);
      free(line);
      line = get_next_line(fd);
      continue;
    }
    if (store_and_parse_map(line, data))
      return (close(fd), 1);
    // return (close(fd), free(line), 1);
    // free(line);
    line = get_next_line(fd);
  }
  if (!cheack_walls(data))
    return (print_error("pMap not Valid\n", RED), 1);
  printf("line = %s\n", line);
  // if (player_around_walls(data))
  //   return (print_error("Player is around wall\n", RED), 1);
  init_the_player(data);
  close(fd);
  return (0);
}
