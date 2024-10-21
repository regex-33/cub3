#include "../inc/parsing.h"

// mobrhanat viragorse
float distance(float x0, float y0, float x1, float y1, float angle) {
  (void)angle;
  return (sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));
}

void draw_line_dda(t_game *game, int x0, int y0, int x1, int y1, int color) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

  float x_inc = dx / (float)steps;
  float y_inc = dy / (float)steps;

  float x = x0;
  float y = y0;

  for (int i = 0; i <= steps; i++) {
    my_mlx_pixel_put(&game->img, x, y, color);
    x += x_inc;
    y += y_inc;
  }
}

float ft_abs(float n) {
  if (n < 0)
    return -n;
  return n;
}

void draw_arrow(t_game *game, int x, int y, float angle, int size, int color) {
  int x3 = x + cos(angle) * size;
  int y3 = y + sin(angle) * size;

  int x2 = x + cos(angle + 2.5) * (size / 2);
  int y2 = y + sin(angle + 2.5) * (size / 2);

  int x1 = x + cos(angle - 2.5) * (size / 2);
  int y1 = y + sin(angle - 2.5) * (size / 2);

  draw_line_dda(game, x1, y1, x2, y2, color);
  draw_line_dda(game, x2, y2, x3, y3, color);
  draw_line_dda(game, x3, y3, x1, y1, color);
}

void put_player_image(t_game *game, int x, int y, float angle) {
  (void)angle;
  (void)x;
  (void)y;
  mlx_put_image_to_window(game->mlx, game->win, game->imposter.img_back, 0, 0);
  // sleep(1);
}

void draw_minimap_player(t_game *game) {
  int player_x = game->player->plyr_x * MINIMAP_SCALE;
  int player_y = game->player->plyr_y * MINIMAP_SCALE;
  double angle = game->player->angle;
  int arrow_x = player_x + cos(angle) * MINIMAP_PLAYER_SIZE;
  int arrow_y = player_y + sin(angle) * MINIMAP_PLAYER_SIZE;
  (void)arrow_x;
  (void)arrow_y;

  // printf("angle: %f\n", angle);
  //  if palyer is angle is 0 to 180 and 360 to 180
  // 2 * M_PI - M_PI / 2 is 270 and M_PI is 270
  // if ((angle >= 0 && angle <= M_PI) ||
  // (angle >= 2 * M_PI - M_PI / 2 && angle <= 2 * M_PI))
  //     angle -= M_PI;
  // 3 * M_PI / 2 is 270
  if (angle >= M_PI / 2 && angle <= 3 * M_PI / 2)
    angle -= M_PI;
  // 270 to 90
  angle = fmod(angle + 2 * M_PI, 2 * M_PI);
  if (angle >= 3 * M_PI / 2 && angle <= 2 * M_PI)
    angle += M_PI;
  angle = fmod(angle + 2 * M_PI, 2 * M_PI);

  // if player angle is 180 to 360
  // 2 * M_PI - M_PI / 2 is 270
  if (player_x > 20 && player_x < 40)
    player_x -= 6;
  game->pxl_x = player_x;
  if (player_y > 20)
    player_y -= 6;
  else
    player_y += 6;
  // if (player_y > 30)
  //     player_y -= 10;
  game->pxl_y = player_y;
  // printf("player_x: %d, player_y: %d\n", player_x, player_y);
  // put_player_image(game, player_x, player_y, angle);

  // draw_arrow(game, player_x, player_y, angle, 6, MINIMAP_COLOR_PLAYER);
}

void draw_minimap(t_game *game) {
  draw_map2d(game);
  draw_minimap_player(game);
}

int cast_rays(t_game *game) {
  int r, mx, my, mp, dof;
  float vx, vy, rx, ry, ra, xo, yo, disV, disH, Tan;
  // ra = game->player->angle - (game->player->fov_rd / 2);
  ra = game->player->angle - 30 * DR;
  ra = fmod(ra + 2 * M_PI, 2 * M_PI);
  for (r = 0; r < S_W; r++) {
    // check vertical lines
    dof = 0, disV = 100000;
    Tan = tan(ra);
    if (cos(ra) > 0.001) {
      rx = (((int)game->player->plyr_x >> 6) << 6) + 64;
      ry = (game->player->plyr_x - rx) * Tan + game->player->plyr_y;
      xo = 64;
      yo = -xo * Tan;
    } // looking left
    else if (cos(ra) < -0.001) {
      rx = (((int)game->player->plyr_x >> 6) << 6) - 0.0001;
      ry = (game->player->plyr_x - rx) * Tan + game->player->plyr_y;
      xo = -64;
      yo = -xo * Tan;
    } // looking right
    else {
      rx = game->player->plyr_x;
      ry = game->player->plyr_y;
      dof = 8;
    } // looking up or down. no hit

    while (dof < game->map.w_map) {
      mx = (int)(rx) >> 6;
      my = (int)(ry) >> 6;
      mp = my * game->map.w_map + mx;
      if (mx < 0 || mx >= game->map.w_map || my < 0 || my >= game->map.h_map)
        mp = 0;
      if (mp > 0 && mp < game->map.w_map * game->map.h_map &&
          game->map.map[my][mx] == '1') {
        dof = game->map.w_map;
        disV = cos(ra) * (rx - game->player->plyr_x) -
               sin(ra) * (ry - game->player->plyr_y);
      } else {
        rx += xo;
        ry += yo;
        dof += 1;
      } // check next horizontal
    }
    vx = rx;
    vy = ry;

    //---Horizontal---

    dof = 0;
    disH = 100000;
    Tan = 1.0 / Tan;
    if (sin(ra) > 0.001) {
      ry = (((int)game->player->plyr_y >> 6) << 6) - 0.0001;
      rx = (game->player->plyr_y - ry) * Tan + game->player->plyr_x;
      yo = -64;
      xo = -yo * Tan;
    } // looking up
    else if (sin(ra) < -0.001) {
      ry = (((int)game->player->plyr_y >> 6) << 6) + 64;
      rx = (game->player->plyr_y - ry) * Tan + game->player->plyr_x;
      yo = 64;
      xo = -yo * Tan;
    } // looking down
    else {
      rx = game->player->plyr_x;
      ry = game->player->plyr_y;
      dof = 8;
    } // looking straight left or right

    while (dof < game->map.h_map) {
      mx = (int)(rx) >> 6;
      my = (int)(ry) >> 6;
      mp = my * game->map.w_map + mx;
      if (mx < 0 || mx >= game->map.w_map || my < 0 || my >= game->map.h_map)
        mp = 0;
      if (mp > 0 && mp < game->map.w_map * game->map.h_map &&
          game->map.map[my][mx] == '1') {
        dof = game->map.h_map;
        disH = cos(ra) * (rx - game->player->plyr_x) -
               sin(ra) * (ry - game->player->plyr_y);
      } else {
        rx += xo;
        ry += yo;
        dof += 1;
      } // check next horizontal
    }
    if (disV < disH) {
      rx = vx;
      ry = vy;
      disH = disV;
    }
    game->ray->distance = ft_abs(disH);
    // draw_line_dda(game, game->player->plyr_x , game->player->plyr_y, rx, ry,
    // 0x00FF00FF); ca is the angle between the player direction and the ray
    // direction
    game->ray->distance *= cos(nor_angle(game->player->angle - ra));
    if (game->ray->distance <= 0) {
      game->ray->distance = 1;
    }
    int wall_h, b_pix, t_pix;
    wall_h = (TILE_SIZE / game->ray->distance) *
             ((S_W / 2) / tan(game->player->fov_rd / 2));
    b_pix = (S_H / 2) + (wall_h / 2); // get the bottom pixel
    t_pix = (S_H / 2) - (wall_h / 2); // get the top pixel
    if (b_pix > S_H)                  // check the bottom pixel
      b_pix = S_H;
    if (t_pix < 0) // check the top pixel
      t_pix = 0;
    draw_wall(game, r, t_pix, b_pix);          // draw the wall
    draw_floor_ceiling(game, r, t_pix, b_pix); // draw the floor and the ceiling
    ra += nor_angle(game->player->fov_rd / S_W); // next angle
  }
  // draw_map2d(game);
  // draw_minimap(game);
  // put_player(game);
  draw_minimap(game);
  // put_player_image(game, player_x, player_y, angle);
  mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
  mlx_put_image_to_window(game->mlx, game->win, game->imposter.img_back,
                          game->pxl_x, game->pxl_y - 6);
  return 0;
}
