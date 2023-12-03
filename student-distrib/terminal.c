#include "terminal.h"
#include "./devices/i8259.h"

// Store buffer for each terminal (0 for first, 1 for second, etc.)
static unsigned int buffer_idx[3] = {0,0,0};
static uint8_t line_buffer[3][LINE_BUFFER_SIZE];
static volatile int enter_flag_pressed[3] = {0,0,0};
static unsigned int save_buffer_idx[3] = {0,0,0};
int terminal_idx = 0; //active one
int first_shell_started = 0; //flag for first shell
int new_terminal_flag = 0; //flag for starting a new shell
int32_t terminal_pids[3] = {-1, -1, -1}; //pid array for terminal shells

int save_screen_x[3] = {7,0,0};
int save_screen_y[3] = {1,0,0};

/* get_terminal_idx()
 * Inputs: none
 * Return Value: active terminal
 * Function: returns active terminal*/
int get_terminal_idx()
{
    return terminal_idx;
}

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
        line_buffer[terminal_idx][buffer_idx[terminal_idx]] = ascii; //add to line buff
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
    
    int term = terminal_idx;
    backspace(term);
    if(line_buffer[term][buffer_idx[term]] == '\t') // add extra for tab
    {
        backspace(term);
        backspace(term);
        backspace(term);
    }
    line_buffer[term][buffer_idx[term]-1] = 0;
    buffer_idx[term]--;
    
}

/* terminal clear
 * Inputs: none
 * Return Value: none
 * Function: resets buffer_idx */
void terminal_clear() {
    int i;
    for (i = 0; i < LINE_BUFFER_SIZE; i++) {
        line_buffer[terminal_idx][i] = '\0';
    }
    buffer_idx[terminal_idx] = 0;
}

/* is_started
 * Inputs: none
 * Return Value: first_shell_started
 * Function: returns if first shell is starting */
int is_started()
{   
    return first_shell_started;
};


/* terminal_enter
 * Inputs: none
 * Return Value: none
 * Function: saves buffer idx for terminal_read and resets it, sets flag to allow read */
void terminal_enter()
{
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
    first_shell_started = 1;
    int term = terminal_idx;
    while (get_schedule_idx() != terminal_idx || enter_flag_pressed[terminal_idx] != 1){}; //wait for enter
    
    line_buffer[term][save_buffer_idx[term]] = '\n'; 
    save_buffer_idx[term]++;
    enter_flag_pressed[term] = 0;
    
    //memcpy to buf
    if (nbytes < save_buffer_idx[term]) { 
        memcpy(buf, (const void *) line_buffer[term], nbytes);
        return nbytes;
    }
    else
    {
        memcpy(buf, (const void *) line_buffer[term], save_buffer_idx[term]);
        return save_buffer_idx[term];
    }
    
    return -1;
}

/* terminal_write
 * Inputs: fd, buf, nbytes
 * Return Value: num bytes wrote
 * Function: prints the chars in input buf to screen */
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes) {
    int i;
    int term;
    if(!is_terminals_initialized()) //check for startup, before normal scheduling
    {
        term = terminal_idx;
    }
    else
    {
        term = get_schedule_idx();
    }
    
    for (i = 0; i < nbytes; i++) {
        if(((char*)buf)[i] == '\t') // add extra for tab
        {
            putc_terminal(' ', term);
            putc_terminal(' ', term);
            putc_terminal(' ', term);
            putc_terminal(' ', term);
        }
        else
            putc_terminal(((char*)buf)[i], term);
    }
    return nbytes;
    
}

/* get_saved_screen_x
 * Inputs: term
 * Return Value: screen x
 * Function: returns screen x for input terminal */
int get_saved_screen_x(int term)
{
    return save_screen_x[term];
}

/* get_saved_screen_y
 * Inputs: term
 * Return Value: screen y
 * Function: returns screen y for input terminal */
int get_saved_screen_y(int term)
{
    return save_screen_y[term];
}

/* set_saved_screen_x
 * Inputs: term, x
 * Return Value: NA
 * Function: sets screen x for input terminal */
void set_saved_screen_x(int term, int x)
{
    save_screen_x[term] = x;
}

/* set_saved_screen_y
 * Inputs: term, y
 * Return Value: NA
 * Function: sets screen y for input terminal */
void set_saved_screen_y(int term, int y)
{
    save_screen_y[term] = y;
}

/* terminal_switch
 * Inputs: t_idx
 * Return Value: none
 * Function: copies and swaps video mem for next process*/
void terminal_switch (int t_idx)
{
    if(t_idx > 2 || t_idx < 0 || new_terminal_flag == 1) return;


    video_memory_page_table[VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx].base_31_12 = VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx; // unlink page
    flush_tlb();
    memcpy((void *) (VIDEO + FOUR_KB * (terminal_idx + 1)), (void *) VIDEO, 4000); // copy current to storage
    
    terminal_idx = t_idx;

    memcpy((void *) VIDEO, (void *) (VIDEO + FOUR_KB * (terminal_idx + 1)), 4000);
    video_memory_page_table[VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx].base_31_12 = VIDEO_ADDRESS / FOUR_KB;
    flush_tlb();
    
    set_vid_mem(terminal_idx); // update cursor this function is stupid
}

/* init_terminals_vidmaps
 * Inputs: NA
 * Return Value: none
 * Function: sets up user vidmap pages*/
void init_terminals_vidmaps()
{
    // 8kb to 20kb is terminal vmem
    video_memory_page_table[1 + (VIDEO_ADDRESS / FOUR_KB)].p = 1; 
    video_memory_page_table[1 + (VIDEO_ADDRESS / FOUR_KB)].us = 1;
    video_memory_page_table[1 + (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = VIDEO / FOUR_KB; 
    video_memory_page_table[2 + (VIDEO_ADDRESS / FOUR_KB)].p = 1; 
    video_memory_page_table[2 + (VIDEO_ADDRESS / FOUR_KB)].us = 1;
    video_memory_page_table[2 + (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = 2 + (VIDEO_ADDRESS / FOUR_KB);
    video_memory_page_table[3 + (VIDEO_ADDRESS / FOUR_KB)].p = 1; 
    video_memory_page_table[3 + (VIDEO_ADDRESS / FOUR_KB)].us = 1;
    video_memory_page_table[3 + (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = 3 + (VIDEO_ADDRESS / FOUR_KB);
    flush_tlb();
}

/* get_terminal_arr
 * Inputs: idx
 * Return Value: pid
 * Function: returns terminal pid*/
int get_terminal_arr(int index) {
    if (index < 0 || index > 2) return -1;
    return terminal_pids[index];
}

/* set_terminal_arr
 * Inputs: idx, pid val
 * Return Value: none
 * Function: sets terminal pid*/
void set_terminal_arr(int index, int val) {
    terminal_pids[index] = val;
}
