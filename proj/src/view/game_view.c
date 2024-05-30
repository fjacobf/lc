#include <lcom/lcf.h>

#include "game_view.h"

#include "../model/game.h"

#include "ball_view.h"
#include "sprite_view.h"
#include "wall_view.h"
#include "cursor_view.h"

extern Ball *ball;
extern Wall *wall_1;
extern Wall *wall_2;
extern Wall *wall_mouse;
extern Cursor *cursor_game;

#define WALL_COLOR1 0xFFFF00
#define WALL_COLOR2 0xA020F0
#define WALL_COLOR_MOUSE 0xFF0000

extern uint32_t score;
extern uint8_t multiplier;

extern uint16_t x_max;
extern uint16_t y_max;

int draw_game() {
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

void update_time_on_screen() {
  // Get the current time from the RTC
  uint32_t hours, minutes, seconds;

  sys_outb(RTC_ADDR_REG, RTC_HOURS);
  sys_inb(RTC_DATA_REG, &hours);

  sys_outb(RTC_ADDR_REG, RTC_MINUTES);
  sys_inb(RTC_DATA_REG, &minutes);

  sys_outb(RTC_ADDR_REG, RTC_SECONDS);
  sys_inb(RTC_DATA_REG, &seconds);

  // Convert BCD to binary if necessary
  hours = (hours >> 4) * 10 + (hours & 0x0F);
  minutes = (minutes >> 4) * 10 + (minutes & 0x0F);
  seconds = (seconds >> 4) * 10 + (seconds & 0x0F);

  // Draw the time on the screen (adjust coordinates as necessary)
  draw_time(hours, minutes, seconds, x_max * 0.5, y_max * 0.05);
}

void draw_time(uint32_t hours, uint32_t minutes, uint32_t seconds, int x, int y) {
  char time_str[9];
  sprintf(time_str, "%02u:%02u:%02u", hours, minutes, seconds);

  // Draw the time string on the screen
  vg_draw_string(x, y, time_str, COLOR_WHITE);
}
