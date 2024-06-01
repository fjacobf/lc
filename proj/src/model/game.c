#include <lcom/lcf.h>

#include "game.h"

#include "ball.h"
#include "sprite.h"
#include "wall.h"

#include "../controller/keyboard.h"
#include "../controller/mouse.h"

extern scancode code;
extern struct packet mouse_packet;

Ball *ball = NULL;
Wall *wall_1 = NULL;
Wall *wall_2 = NULL;
Wall *wall_mouse = NULL;

uint16_t x_max;
uint16_t y_max;

uint32_t score;
uint8_t multiplier;

static uint32_t counter;
static uint32_t speedup;

int16_t x;
int16_t y;

int start_game(uint16_t xResolution, uint16_t yResolution, uint8_t difficulty) {
  uint8_t direction = (rand() % 2) ? 1 : -1; // ball starting direction may be left or right
  ball = construct_ball(xResolution / 2, yResolution / 2, difficulty * direction, difficulty * direction);

  if (!ball) {
    printf("%s: construct_ball(%d, %d, %d, %d) error\n", __func__, xResolution / 2, yResolution / 2, difficulty * direction, difficulty * direction);
    return 1;
  }

  wall_1 = construct_wall(0, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);

  if (!wall_1) {
    printf("%s: construct_wall(%d, %d, %d, %d) error\n", __func__, 0, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);
    return 1;
  }

  wall_2 = construct_wall(xResolution - 10, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);

  if (!wall_2) {
    printf("%s: construct_wall(%d, %d, %d, %d) error\n", __func__, xResolution - 10, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);
    return 1;
  }

  wall_mouse = construct_wall(xResolution / 2, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);

  if (!wall_mouse) {
    printf("%s: construct_wall(%d, %d, %d, %d) error\n", __func__, xResolution / 2, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);
    return 1;
  }
  
  if (construct_numbers()) {
    printf("%s: construct_numbers() error\n", __func__);
    return 1;
  }

  if(construct_background()){
    printf("%s: construct_background() error\n", __func__);
    return 1;
  }

  y = wall_mouse->y;
  x_max = xResolution;
  y_max = yResolution;
  score = 0;
  multiplier = difficulty;
  counter = 0;
  speedup = 600 * multiplier;

  return 0;
}

void timer_game_handler() {
  counter++;
  int16_t y = ball->y + ball->sprite->height / 2; // ordenada do ponto mais à esquerda da bola
  if (ball->x <= wall_1->x + wall_1->w && y >= wall_1->y && y <= wall_1->y + wall_1->h) {
    // colisão da bola com a parede
    score += multiplier;
    move_ball_after_collision_with_wall(ball, wall_1);
  }

  if (ball->x + ball->sprite->width >= wall_2->x && y >= wall_2->y && y <= wall_2->y + wall_2->h) {
    // colisão da bola com a parede
    score += multiplier;
    move_ball_after_collision_with_wall(ball, wall_2);
  }

  if (ball->x + ball->sprite->width >= wall_mouse->x && ball->x <= wall_mouse->x + wall_mouse->w && y >= wall_mouse->y && y <= wall_mouse->y + wall_mouse->h) {
    // colisão da bola com a parede
    score += multiplier;
    move_ball_after_collision_with_wall(ball, wall_mouse);
  }

  move_ball(ball, x_max, y_max);
  if (counter % speedup == 0 && !speedup_ball(ball)) {
    multiplier++;
    speedup  = 600 * multiplier;
    counter = 0;
  }
}

void keyboard_game_handler() {
  if ((code.size == 1 && code.bytes[0] == KBD_W_MAKECODE))
    move_wall_up(wall_1);
  if ((code.size == 1 && code.bytes[0] == KBD_S_MAKECODE))
    move_wall_down(wall_1, y_max);
  
  if((code.size == 2 && code.bytes[1] == KBD_ARROW_UP_MAKECODE_LSB))
    move_wall_up(wall_2);
  
  if((code.size == 2 && code.bytes[1] == KBD_ARROW_DOWN_MAKECODE_LSB))
    move_wall_down(wall_2, y_max);
}

void mouse_game_handler() {
  y -= mouse_packet.delta_y;
  if (y < 0)                                         // colisão com o limite superior do ecrã
    y = 0;
  else if (y + wall_mouse->h >= y_max) // colisão com o limite inferior do ecrã
    y = y_max - wall_mouse->h - 1;

  move_wall_y(wall_mouse, y);

  if (mouse_packet.rb && !speedup_ball(ball)) {
    multiplier++;
    speedup  = 600 * multiplier;
    counter = 0;
  }
}

bool check_game_over() {
  if(ball->x <= wall_1->x && (ball->y < wall_1->y || ball->y > wall_1->y + wall_1->h)){
    return true;
  }
  if(ball->x + ball->sprite->width >= wall_2->x && (ball->y < wall_2->y || ball->y > wall_2->y + wall_2->h)){
    return true;
  }
  return false;
}

void end_game() { 
  destroy_ball(ball);
  destroy_wall(wall_1);
  destroy_wall(wall_2);
  destroy_numbers();
  destroy_wall(wall_mouse);
  destroy_background();
}
