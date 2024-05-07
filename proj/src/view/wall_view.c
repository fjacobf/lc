#include <lcom/lcf.h>

#include "wall_view.h"

#include "../controller/video.h"

#define WALL_COLOR1 0xFFFF00
#define WALL_COLOR2 0xA020F0

int c = 0;

int draw_wall(Wall *wall) {
  if (!wall) {
    printf("%s: wall is NULL\n", __func__);
    return 1;
  }

  uint32_t color = c % 2 == 0 ? WALL_COLOR1 : WALL_COLOR2;
  if (video_draw_rectangle(wall->x, wall->y, wall->w, wall->h, color)) {
    printf("%s: video_draw_rectangle(wall->x; %d, wall->y: %d, wall->w: %d, wall->h: %d, WALL_COLOR: 0x%x) error\n", __func__, wall->x, wall->y, wall->w, wall->h, color);
    return 1;
  }

  c +=1;
  return 0;
}
