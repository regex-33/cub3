#include "../inc/parsing.h"

int ft_cout_lines_begin_with(char *path, char c) {
  int fd;
  char *line;
  int count;

  count = 0;
  fd = open(path, O_RDONLY);
  if (fd == -1)
    return (perror(PROGRAM_NAME), -1);
  line = get_next_line(fd);
  while (line) {
    if (line[0] == c)
      count++;
    free(line);
    line = get_next_line(fd);
  }
  close(fd);
  return (count);
}

int map_extention(char *path) {
  int len;

  len = ft_strlen(path);

  if (len < 4)
    return (1);
  if (ft_strncmp(path + len - 4, ".cub", 4))
    return (1);
  return (0);
}

void ft_replace(char *line, char s, char r) {
  while (*line != '\0') {
    if (*line == s)
      *line = r;
    line++;
  }
}

int number_of_words(char **str) {
  int i;

  i = 0;
  while (str[i])
    i++;
  return (i);
}

int map_line_is_valid(char *line) {
  int i;

  i = 0;
  while (line[i] != '\0') {
    if (line[i] != '1' && line[i] != '0')
      return (0);
    i++;
  }
  return (1);
}

void print_error(char *error, char *color) {
  ft_putstr_fd(ERROR, 2);
  ft_putstr_fd(color, 2);
  ft_putstr_fd(error, 2);
  ft_putstr_fd(RESET, 2);
}
