#include "terminal.h"
#include "lib.h"

static unsigned int buffer_idx = 0;
static char line_buffer[LINE_BUFFER_SIZE];
static volatile int enter_flag_pressed = 0;
static unsigned int save_buffer_idx = 0;

/* get_buffer_Fill
 * Inputs: none
 * Return Value: buffer_idx
 * Function: returns the next empty idx in the buffer (0 indexed) */
int get_buffer_fill()
{
    return buffer_idx;
}

/* write_to_terminal
 * Inputs: char ascii
 * Return Value: none
 * Function: addes the current char to the buffer */
void write_to_terminal(unsigned char ascii) {
    if(buffer_idx == 128)
        return;
    else
    {
        line_buffer[buffer_idx] = ascii;
        buffer_idx++;
    }
           
}
/* terminal_backspace
 * Inputs: none
 * Return Value: none
 * Function: removes the last char in the buffer */
void terminal_backspace()
{
    line_buffer[buffer_idx-1] = 0;
    buffer_idx--;
}

/* terminal clear
 * Inputs: none
 * Return Value: none
 * Function: resets buffer_idx */
void terminal_clear() {
    buffer_idx = 0;
    // TODO: clear buffer
}

/* terminal_enter
 * Inputs: none
 * Return Value: none
 * Function: saves buffer idx for terminal_read and resets it, sets flag to allow read */
void terminal_enter()
{
    enter_flag_pressed = 1;
    save_buffer_idx = buffer_idx;
    buffer_idx = 0;
}

/* terminal_open
 * Inputs: filename
 * Return Value: 0 sucess always
 * Function: sets terminal start setttings */
int32_t terminal_open(const uint8_t * filename) {
    // TODO: may want to init buf to 0s, but that's handled anyways, so not necessary
    buffer_idx = 0;
    return 0;
}

/* terminal_close
 * Inputs: fd
 * Return Value: 0 for sucess always
 * Function: resets buffer_idx */
int32_t terminal_close(int32_t fd) {
    buffer_idx = 0;
    return 0;
}

/* terminal_read
 * Inputs: fd, buf, nbytes
 * Return Value: number of bytes read, -1 for fail
 * Function: waits until enter is pressed then writes all bytes in buffer(including new ling) to input buf */
int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes) {
    // NOTE: this is a blocking call, so it can't be interrupted
    while (enter_flag_pressed != 1);
    
    cli();
    
    line_buffer[save_buffer_idx] = '\n';
    save_buffer_idx++;
    enter_flag_pressed = 0;
    //save_buffer_idx = buffer_idx;
    if (nbytes < save_buffer_idx) {
        memcpy(buf, line_buffer, nbytes);
        sti();
        return nbytes;
    }
    else
    {
        memcpy(buf, line_buffer, save_buffer_idx);
        sti();
        return save_buffer_idx;
    }
    // should check for ENTER and BACKSPACE here
    sti();
    return -1;
}

/* terminal_write
 * Inputs: fd, buf, nbytes
 * Return Value: num bytes wrote
 * Function: prints the chars in input buf to screen */
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes) {
    int i;
    cli();
    for (i = 0; i < nbytes; i++) {
        putc(((char*)buf)[i]);
    }
    sti();
    return nbytes;
}
