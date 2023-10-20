#include "types.h"

#ifndef _TERMINAL_H
#define _TERMINAL_H

#define LINE_BUFFER_SIZE 128



// TODO: edge case where CAPS LOCK is turned on before starting the kernel
// TODO: may need to add cli, sti since this is a crit section
// CTRL | SHIFT | CAPS LOCK

extern void write_to_terminal(unsigned char ascii);
void terminal_clear();
int get_buffer_fill();
void terminal_backspace();

extern int32_t terminal_open(const uint8_t * filename);
extern int32_t terminal_close(int32_t fd);
extern int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes);

static int enter_flag_pressed = 0;

#endif /* _TERMINAL_H */
