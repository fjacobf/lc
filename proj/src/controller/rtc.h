#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>

void rtc_int_handler();
int rtc_enable_update_interrupts();
int rtc_subscribe_int(uint8_t *bit_no);
int rtc_unsubscribe_int();

#endif /* _RTC_H_ */
