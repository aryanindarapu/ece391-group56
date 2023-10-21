#include "terminal.h"
#include "lib.h"

static unsigned int buffer_idx = 0;
static char line_buffer[LINE_BUFFER_SIZE];
static volatile int enter_flag_pressed = 0;

int get_buffer_fill()
{
    return buffer_idx;
}

void write_to_terminal(unsigned char ascii) {
    if(buffer_idx == 128)
        return;
    else
    {
        line_buffer[buffer_idx] = ascii;
        buffer_idx++;
    }
           
}

void terminal_backspace()
{
    line_buffer[buffer_idx-1] = 0;
    buffer_idx--;
}

void terminal_clear() {
    buffer_idx = 0;
    // TODO: clear buffer
}
 
void terminal_enter()
{
    enter_flag_pressed = 1;
}

// New functions, don't want to remove old ones
// TODO: comment
int32_t terminal_open(const uint8_t * filename) {
    // TODO: may want to init buf to 0s, but that's handled anyways, so not necessary
    buffer_idx = 0;
    return 0;
}

int32_t terminal_close(int32_t fd) {
    buffer_idx = 0;
    return 0;
}

// reads from keyboard buffer into buf TODO: comment
int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes) {
    // NOTE: this is a blocking call, so it can't be interrupted
    int save_buffer_idx;
    while (enter_flag_pressed != 1);
    
    cli();
    
    line_buffer[buffer_idx] = '\n';
    buffer_idx++;
    enter_flag_pressed = 0;
    save_buffer_idx = buffer_idx;
    if (nbytes < buffer_idx) {
        memcpy(buf, line_buffer, nbytes);
        buffer_idx = 0;
        sti();
        return nbytes;
    }
    else
    {
        memcpy(buf, line_buffer, buffer_idx);
        buffer_idx = 0;
        sti();
        return save_buffer_idx;
    }
    // should check for ENTER and BACKSPACE here
    sti();
    return -1;
}

// writes to screen from buf
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes) {
    int i;
    cli();
    for (i = 0; i < nbytes; i++) {
        putc(((char*)buf)[i]);
    }
    sti();
    return nbytes;
} 