#define RTC_PORT_COMMAND    0x70
#define RTC_PORT_DATA       0x71

// Intialize the RTC
void init_rtc();

// Handles the RTC when it is called in an interupt
void rtc_handler();
