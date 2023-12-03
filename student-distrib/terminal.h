#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "lib.h"
#include "file_system_driver.h"
#include "paging.h"
#include "syscall.h"
// #include "process.h"

#define LINE_BUFFER_SIZE 128
#define VIDEO       0xB8000

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
/* fnc to swap terminals */
void terminal_switch(int t_idx);
/* init pages to hold vid data for 3 terms */
void init_terminals_vidmaps();
/* returns the pid of the input terminal index */
int get_terminal_arr(int index);
/* sets the pid of the input terminal index */
void set_terminal_arr(int index, int val);
/* returns whether or not any shell has started*/
int is_started();
/* returns active terminal */
int get_terminal_idx();

/* get/set cursor pos for each terminal */
int get_saved_screen_x(int term);
int get_saved_screen_y(int term);
void set_saved_screen_x(int term, int x);
void set_saved_screen_y(int term, int y);


/* standard file operations for terminal */
extern int32_t terminal_open(const uint8_t* filename);
extern int32_t terminal_close(int32_t fd);
extern int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes);

#endif /* _TERMINAL_H */
