#include "types.h"

#ifndef _TERMINAL_H
#define _TERMINAL_H

#define LINE_BUFFER_SIZE 128
#define NUM_SPECIAL_FLAGS 3

#define CAPS_LOCK_P 58
#define CTRL_P 29
#define CTRL_R 157
#define SHIFT_P 42
#define SHIFT_R 170

#define CTRL_INDEX 0 
#define SHIFT_INDEX 1
#define CAPS_LOCK_INDEX 2
// #define ALT_INDEX 2 


// TODO: edge case where CAPS LOCK is turned on before starting the kernel
// TODO: may need to add cli, sti since this is a crit section
// CTRL | SHIFT | CAPS LOCK
static char line_buffer[LINE_BUFFER_SIZE];

extern void write_to_terminal(unsigned char keycode);
void terminal_clear();

extern int32_t terminal_open(const uint8_t * filename);
extern int32_t terminal_close(int32_t fd);
extern int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes);

#endif /* _TERMINAL_H */
