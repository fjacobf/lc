#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "rtc.h"

static int rtc_hook_id = 8;

int rtc_subscribe_int(uint8_t *bit_no) {
  *bit_no = rtc_hook_id;
  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id) != OK)
    return 1;
  if (sys_irqenable(&rtc_hook_id) != OK)
    return 1;
  return 0;
}

int rtc_unsubscribe_int() {
  if (sys_irqdisable(&rtc_hook_id) != OK)
    return 1;
  if (sys_irqrmpolicy(&rtc_hook_id) != OK)
    return 1;
  return 0;
}

int rtc_enable_update_interrupts() {
  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  uint32_t reg_b;
  sys_inb(RTC_DATA_REG, &reg_b);
  reg_b |= RTC_UIE; // Enable Update Interrupt
  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_outb(RTC_DATA_REG, reg_b);
  return 0;
}

void rtc_int_handler() {
  // Read from the RTC to clear the interrupt
  sys_outb(RTC_ADDR_REG, RTC_REG_C);
  uint32_t reg_c;
  sys_inb(RTC_DATA_REG, &reg_c);

  // Call function to update time on the screen
  update_time_on_screen();
}
