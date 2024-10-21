#include "../inc/parsing.h"
#include <stdlib.h>

int ft_colorValue(char *str) {

  int result;

  result = 0;
  while ((*str >= 9 && *str <= 13) || *str == 32)
    str++;
  while (*str) {
    if (*str >= '0' && *str <= '9') {
      result = result * 10 + (*str - 48);
      if (result > 255)
        return -1;
    }
    str++;
  }
  return (result);
}

int validate_color(t_game *data, char *color_str) {
  int i;
  int color = 0;
  int len;

  i = 0;
  len = ft_strlen(color_str);
  if (color_str[len - 1] == '\n')
    color_str[len - 1] = '\0';
  while (color_str[i] != '\0') {
    if (color_str[i] < '0' || color_str[i] > '9')
      return (print_error("Color value must be a number\n", RED),
              free_garbage(data->garbage), exit(1), -1);
    i++;
  }
  color = ft_colorValue(color_str);
  if (color < 0)
    return (print_error("Color value must be between 0 and 255\n", RED),
            free_garbage(data->garbage), exit(1), -1);
  return (color);
}

void print_2d(char **str) {
  int i = 0;

  while (str[i]) {
    printf("str[%d] = %s\n", i, str[i]);
    i++;
  }
}

int process_color(t_game *data, char *color_str, int *color_dest,
                  char *error_msg) {
  char **temp;
  char *commas;
  int red = 0;
  int green = 0;
  int blue = 0;

  temp = ft_split(color_str, ',');
  if (!temp)
    return (perror(PROGRAM_NAME), 0);
  commas = ft_strtrim(color_str, ",");
  if (number_of_words(temp) != 3 || ft_strcmp(commas, color_str))
    return (print_error(error_msg, RED), free2d(temp), free(commas), 0);
  red = validate_color(data, temp[0]);
  green = validate_color(data, temp[1]);
  blue = validate_color(data, temp[2]);
  if (red == -1 || green == -1 || blue == -1)
    return (free2d(temp), free(commas), 0);
  if (*color_dest)
    return (print_error("Color already exists\n", RED), free2d(temp),
            free(commas), 0);
  *color_dest = (red << 16) | (green << 8) | blue;
  return (free2d(temp), free(commas), 1);
}

int extract_colors(char *line, t_game *data) {
  char **split = NULL;
  int result;

  result = -1;
  split = ft_split(line, ' ');
  if (!split)
    return (perror(PROGRAM_NAME), 0);
  if (!ft_strcmp(split[0], "F"))
    result = process_color(data, split[1], &data->map.floor_color,
                           "F color format is F R,G,B\n");
  else if (!ft_strcmp(split[0], "C"))
    result = process_color(data, split[1], &data->map.ceiling_color,
                           "C color format is C R,G,B\n");
  return (free2d(split), result);
}
