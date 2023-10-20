#include "../types.h"

#define RTC_PORT_COMMAND    0x70
#define RTC_PORT_DATA       0x71

int32_t rtc_read(int32_t fd, void * buf, int32_t nbytes);

int32_t rtc_write(int32_t fd, const void * buf, int32_t nbytes);

int32_t rtc_open(const uint8_t * filename);

int32_t rtc_close(int32_t fd);

// Intialize the RTC
void init_rtc();

// Handles the RTC when it is called in an interrupt
void rtc_handler();
