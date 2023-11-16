#ifndef _ASM_LINKAGE_H
#define _ASM_LINKAGE_H

#include "devices/keyboard.h"
#include "devices/rtc.h"
#include "devices/pit.h"
#include "paging.h"

/* Linkage functions */
extern void keyboard_handler_linkage();
extern void rtc_handler_linkage();
extern void pit_handler_linkage();
#endif
    