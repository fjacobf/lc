#include <lcom/lcf.h>

#include "menu_view.h"

#include "../model/menu.h"

#include "button_view.h"
#include "cursor_view.h"
#include "sprite_view.h"
#include "controller/rtc.h"

extern Button *play;
extern Button *quit;
extern Cursor *cursor_menu;
extern real_time_info time_info; 

extern uint16_t x_max;
extern uint16_t y_max;

int draw_menu() {

  if (draw_time(time_info.hours, (x_max * 0.5)-17, y_max * 0.5)) {
    printf("error");
    return 1;
  }

  if (draw_time(time_info.minutes, (x_max * 0.5)+25, y_max * 0.5)) {
    printf("error");
    return 1;
  }

  if (draw_colon(x_max*0.5-8, (y_max*0.5) + 6)) {
    printf("draw_colon error\n");
    return 1;
  }

  if (draw_button(play)) {
    printf("%s: draw_button(play) error\n", __func__);
    return 1;
  }

  if (draw_button(quit)) {
    printf("%s: draw_button(quit) error\n", __func__);
    return 1;
  }


  if (draw_cursor(cursor_menu)) {
    printf("%s: draw_cursor(cursor_menu) error\n", __func__);
    return 1;
  }

  return 0;
}
