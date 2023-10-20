#include "terminal.h"
#include "lib.h"

static unsigned int buffer_idx = 0;
static char line_buffer[LINE_BUFFER_SIZE];


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
 

// New functions, don't want to remove old ones
// TODO: comment
int32_t terminal_open(const uint8_t * filename) {
    // TODO: may want to init buf to 0s, but that's handled anyways, so not necessary
    return 0;
}

int32_t terminal_close(int32_t fd) {
    return 0;
}

// reads from keyboard buffer into buf
int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes) {
    // NOTE: this is a blocking call, so it can't be interrupted

    while (!enter_flag_pressed);

    // TODO: this is not the full implementation, but we should do somthing like this
    cli();
    if (nbytes < LINE_BUFFER_SIZE) {
        memcpy(buf, line_buffer, nbytes);
        buffer_idx = 0; // clear buffer
    }
    // should check for ENTER and BACKSPACE here
    sti();
}

// writes to screen from buf
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        putc(((char*)buf)[i]);
    }

    return nbytes;
} 