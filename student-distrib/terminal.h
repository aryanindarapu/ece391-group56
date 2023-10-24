#include "types.h"

#ifndef _TERMINAL_H
#define _TERMINAL_H

#define LINE_BUFFER_SIZE 128

/* kbd input to terminal to update buffer */
extern void write_to_terminal(unsigned char ascii);
/* clears terminal */
void terminal_clear();
/* get fnc for buffer fill status */
int get_buffer_fill();
/* remove char from buffer */
void terminal_backspace();
/* fnc to handle enter press */
void terminal_enter();

/* standard file operations for terminal */
extern int32_t terminal_open(void);
extern int32_t terminal_close(int32_t fd);
extern int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes);

#endif /* _TERMINAL_H */
