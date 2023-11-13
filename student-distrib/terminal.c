#include "terminal.h"

// Which terminal we are currently on
static int terminal_idx = 0;

// Store buffer for each terminal (0 for first, 1 for second, etc.)
static unsigned int buffer_idx[3] = {0,0,0};
static uint8_t line_buffer[3][LINE_BUFFER_SIZE];
static uint8_t saved_line_buffer[3][LINE_BUFFER_SIZE];
static volatile int enter_flag_pressed[3] = {0,0,0};
static unsigned int save_buffer_idx[3] = {0,0,0};
char* vidmems[3][4096];
unsigned char terminal_init_check[3] = {1, 0, 0};
int save_screen_x[3];
int save_screen_y[3];

// TODO: add a 2d array of 6 input buffers

/* get_buffer_Fill
 * Inputs: none
 * Return Value: buffer_idx
 * Function: returns the next empty idx in the buffer (0 indexed) */
int get_buffer_fill()
{
    return buffer_idx[terminal_idx];
}

/* write_to_terminal
 * Inputs: char ascii
 * Return Value: none
 * Function: addes the current char to the buffer */
void write_to_terminal(unsigned char ascii) {
    if(buffer_idx[terminal_idx] == 128)
        return;
    else
    {
        line_buffer[terminal_idx][buffer_idx[terminal_idx]] = ascii;
        buffer_idx[terminal_idx]++;
    }
           
}
/* terminal_backspace
 * Inputs: none
 * Return Value: none
 * Function: removes the last char in the buffer */
void terminal_backspace()
{
    if(buffer_idx[terminal_idx]==0) return;
    
    if(line_buffer[terminal_idx][buffer_idx[terminal_idx]] == '\t')
    {
        backspace();
        backspace();
        backspace();
    }

    line_buffer[terminal_idx][buffer_idx[terminal_idx]-1] = 0;

    buffer_idx[terminal_idx]--;
}

/* terminal clear
 * Inputs: none
 * Return Value: none
 * Function: resets buffer_idx */
void terminal_clear() {
    buffer_idx[terminal_idx] = 0;
}


/* terminal_enter
 * Inputs: none
 * Return Value: none
 * Function: saves buffer idx for terminal_read and resets it, sets flag to allow read */
void terminal_enter()
{
    int i;
    for (i = 0; i < LINE_BUFFER_SIZE; i++){
        saved_line_buffer[terminal_idx][i] = line_buffer[i];
    }
    enter_flag_pressed[terminal_idx] = 1;
    save_buffer_idx[terminal_idx] = buffer_idx[terminal_idx];
    buffer_idx[terminal_idx] = 0;
}

/* 
 * terminal_open
 * Inputs: filename
 * Return Value: 0 success always
 * Function: sets terminal start setttings */
int32_t terminal_open(const uint8_t* filename) {
    return -1;
}

/* terminal_close
 * Inputs: fd
 * Return Value: 0 for sucess always
 * Function: resets buffer_idx */
int32_t terminal_close(int32_t fd) {
    return -1;
}

/* terminal_read
 * Inputs: fd, buf, nbytes
 * Return Value: number of bytes read, -1 for fail
 * Function: waits until enter is pressed then writes all bytes in buffer(including new ling) to input buf */
int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes) {
    // NOTE: this is a blocking call, so it can't be interrupted
    // printf("ACCESSED TERMINAL READ");
    sti();
    
    while (enter_flag_pressed[terminal_idx] != 1){sti();};
    
    cli();
    
    line_buffer[terminal_idx][save_buffer_idx[terminal_idx]] = '\n';
    save_buffer_idx[terminal_idx]++;
    enter_flag_pressed[terminal_idx] = 0;
    //save_buffer_idx = buffer_idx;
    if (nbytes < save_buffer_idx[terminal_idx]) {
        memcpy(buf, (const void *) line_buffer[terminal_idx], nbytes);
        sti();
        return nbytes;
    }
    else
    {
        memcpy(buf, (const void *) line_buffer[terminal_idx], save_buffer_idx[terminal_idx]);
        sti();
        return save_buffer_idx[terminal_idx];
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
        if(((char*)buf)[i] == '\t')
        {
            putc(' ');
            putc(' ');
            putc(' ');
            putc(' ');
        }
        else
            putc(((char*)buf)[i]);
    }
    sti();
    return nbytes;
}

void terminal_switch(int t_idx)
{
    if(t_idx > 2 || t_idx < 0) return;
    memcpy((void *) (&vidmems[terminal_idx]), (void *) VIDEO, 4096);
    save_screen_x[terminal_idx] = get_screen_x();
    save_screen_y[terminal_idx] = get_screen_x();

    terminal_idx = t_idx;
    
    memcpy((void *) VIDEO, (void *) (&vidmems[terminal_idx]), 4096);
    save_screen_x[terminal_idx] = get_screen_x();
    save_screen_y[terminal_idx] = get_screen_x();
    
    if(terminal_init_check[terminal_idx] == 0)
    {
        clear();
        terminal_init_check[terminal_idx] = 1;
        sti();
        // execute((const uint8_t *) "shell");
    }
    // printf("aaaa%d", terminal_idx);
    
}

void init_terminals_vidmaps()
{
    // 2kb to 4kb is terminal vmem
    video_memory_page_table[2].p = 1; 
    video_memory_page_table[2].us = 0;
    video_memory_page_table[2].base_31_12 = 2 ; //VIDEO_ADDRESS / FOUR_KB;
    video_memory_page_table[3].p = 1; 
    video_memory_page_table[3].us = 0;
    video_memory_page_table[3].base_31_12 = 3 ; //VIDEO_ADDRESS / FOUR_KB;
    video_memory_page_table[4].p = 1; 
    video_memory_page_table[4].us = 0;
    video_memory_page_table[4].base_31_12 = 4 ; //VIDEO_ADDRESS / FOUR_KB;
    flush_tlb();
}