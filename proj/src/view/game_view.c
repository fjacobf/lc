#include <lcom/lcf.h>

#include "game_view.h"

#include "../model/game.h"

#include "ball_view.h"
#include "sprite_view.h"
#include "wall_view.h"
#include "cursor_view.h"
#include "controller/rtc.h"


extern Ball *ball;
extern Wall *wall_1;
extern Wall *wall_2;
extern Wall *wall_mouse;
extern Cursor *cursor_game;
extern real_time_info time_info; 

#define WALL_COLOR1 0xFFFF00
#define WALL_COLOR2 0xFFA500
#define WALL_COLOR_MOUSE 0xFF0000
#define SUNNY_BACKGROUND 1
#define SUNSET_BACKGROUND 2
#define NIGHT_BACKGROUND 3

extern uint32_t score;
extern uint8_t multiplier;

extern uint16_t x_max;
extern uint16_t y_max;

int draw_game() {

  if(time_info.hours > 5 && time_info.hours < 18){
    if (draw_background(NIGHT_BACKGROUND)) {
      printf("%s: draw_background(x: 0, y: 0)\n", __func__);
      return 1;
    }
  }

  if(time_info.hours >= 18 && time_info.hours < 20){
    if (draw_background(SUNSET_BACKGROUND)) {
      printf("%s: draw_background(x: 0, y: 0)\n", __func__);
      return 1;
    }
  }
  if(time_info.hours >= 20 || time_info.hours <= 5){
    if (draw_background(NIGHT_BACKGROUND)) {
      printf("%s: draw_background(x: 0, y: 0)\n", __func__);
      return 1;
    }
  }

  if (draw_times(x_max * 0.05, y_max * 0.05)) {
    printf("%s: draw_times(x_max * 0.05: %f, y_max * 0.05: %f)\n", __func__, x_max * 0.05, y_max * 0.05);
    return 1;
  }

  if (draw_digit(multiplier, x_max * 0.05 + 20, y_max * 0.05)) {
    printf("%s: draw_digit(multiplier: %d, x_max * 0.05 + 20: %f, y_max * 0.05: %f) error\n", __func__, multiplier, x_max * 0.05 + 20, y_max * 0.05);
    return 1;
  }

  if (draw_number(score, x_max * 0.95, y_max * 0.05)) {
    printf("%s: draw_number(score: %d, x_max * 0.95: %f, y_max * 0.05: %f) error\n", __func__, score, x_max * 0.95, y_max * 0.05);
    return 1;
  }

  if (draw_ball(ball)) {
    printf("%s: draw_ball(ball) error\n", __func__);
    return 1;
  }

  if (draw_wall(wall_1, WALL_COLOR1)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }

  if (draw_wall(wall_2, WALL_COLOR2)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }

  if (draw_wall(wall_mouse, WALL_COLOR_MOUSE)) {
    printf("%s: draw_wall(wall_mouse) error\n", __func__);
    return 1;
  }

  return 0;
}
