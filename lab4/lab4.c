// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"

extern struct packet mouse_packet;
extern uint8_t packet_index;
extern uint32_t counter;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    if (mouse_data_reporting(true)) {
    printf("%s: mouse_data_reporting error\n", __func__);
		return 1;
  }

  uint8_t irq_set;
	if (mouse_subscribe_int(&irq_set)) {
		printf("%s: mouse_subscribe_int error\n", __func__);
		return 1;
	}

	int ipc_status, r;
	message msg;

  while (cnt) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
            /* process it */
            mouse_ih();
            if (packet_index == 3) {
              mouse_print_packet(&mouse_packet);
              if (mouse_restore()) {
                printf("%s: mouse_restore error\n", __func__);
                return 1;
              }
              cnt--;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int()) {
		printf("%s: mouse_unsubscribe_int error\n", __func__);
		return 1;
  }

  if (mouse_data_reporting(false)) {
    printf("%s: mouse_data_reporting error\n", __func__);
		return 1;
  }

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    if (mouse_data_reporting(true)) {
    printf("%s: mouse_data_reporting error\n", __func__);
		return 1;
  }

  uint8_t mouse_irq_set;
	if (mouse_subscribe_int(&mouse_irq_set)) {
		printf("%s: mouse_subscribe_int error\n", __func__);
		return 1;
	}

  uint8_t timer_irq_set;
  if (timer_subscribe_int(&timer_irq_set)) {
    printf("%s: timer_subscribe_int error\n", __func__);
		return 1;
  }

	int ipc_status, r;
	message msg;

	while (counter < idle_time * sys_hz()) {
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
          if (msg.m_notify.interrupts & BIT(timer_irq_set)) { /* Timer0 int? */
            /* process Timer0 interrupt request */
            timer_int_handler();
          }
					if (msg.m_notify.interrupts & BIT(mouse_irq_set)) { /* Mouse int? */
						/* process Mouse interrupt request */
            mouse_ih();
            if (packet_index == 3) {
              mouse_print_packet(&mouse_packet);
              if (mouse_restore()) {
                printf("%s: mouse_restore error\n", __func__);
                return 1;
              }
              counter = 0;
            }
          }
					break;
				default:
					break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

  if (timer_unsubscribe_int()) {
    printf("%s: timer_unsubscribe_int error\n", __func__);
		return 1;
  }

	if (mouse_unsubscribe_int()) {
		printf("%s: mouse_unsubscribe_int error\n", __func__);
		return 1;
  }

  if (mouse_data_reporting(false)) {
    printf("%s: mouse_data_reporting error\n", __func__);
		return 1;
  }

  return 0;

}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){
    if (mouse_data_reporting(true)) {
    printf("%s: mouse_data_reporting error\n", __func__);
		return 1;
  }

  uint8_t irq_set;
	if (mouse_subscribe_int(&irq_set)) {
		printf("%s: mouse_subscribe_int error\n", __func__);
		return 1;
	}

  int ipc_status, r;
	message msg;
  bool exit = false;
  struct mouse_ev* event;

  while (!exit) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(irq_set)) { /* subscribed interrupt */
            /* process it */
            mouse_ih();
            if (packet_index == 3) {
              event = mouse_detect_event_our(&mouse_packet);
              exit = mouse_check_logical_and(event, x_len, tolerance);
              mouse_print_packet(&mouse_packet);
              if (mouse_restore()) {
                printf("%s: mouse_restore error\n", __func__);
                return 1;
              }
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int()) {
		printf("%s: mouse_unsubscribe_int error\n", __func__);
		return 1;
  }

  if (mouse_data_reporting(false)) {
    printf("%s: mouse_data_reporting error\n", __func__);
		return 1;
  }

  return 0;

}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
