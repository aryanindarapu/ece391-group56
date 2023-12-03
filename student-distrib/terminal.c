#include "terminal.h"
#include "process.h"
#include "./devices/i8259.h"

// Store buffer for each terminal (0 for first, 1 for second, etc.)
static unsigned int buffer_idx[3] = {0,0,0};
static uint8_t line_buffer[3][LINE_BUFFER_SIZE];
static uint8_t saved_line_buffer[3][LINE_BUFFER_SIZE];
static volatile int enter_flag_pressed[3] = {0,0,0};
static unsigned int save_buffer_idx[3] = {0,0,0};
int terminal_idx = 0; //active one
int first_shell_started = 0;
int new_terminal_flag = 0;
int temp_terminal_flag = 0;
int32_t terminal_pids[3] = {-1, -1, -1};
// TODO: add rtc switching as well

// char vidmems[3][4096];
int save_screen_x[3] = {7,0,0};
int save_screen_y[3] = {1,0,0};

// TODO: add a 2d array of 6 input buffers

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
    
    int term = terminal_idx;
    backspace(term);
    if(line_buffer[term][buffer_idx[term]] == '\t')
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

// TODO: comment
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
    first_shell_started = 1;
    int term = terminal_idx;
    // sti();
    while (get_schedule_idx() != terminal_idx || enter_flag_pressed[terminal_idx] != 1){};
    
    // while (term !=get_schedule_idx()){};
    
    // cli();
    line_buffer[term][save_buffer_idx[term]] = '\n';
    save_buffer_idx[term]++;
    enter_flag_pressed[term] = 0;
    //save_buffer_idx = buffer_idx;
    if (nbytes < save_buffer_idx[term]) {
        memcpy(buf, (const void *) line_buffer[term], nbytes);
        return nbytes;
    }
    else
    {
        memcpy(buf, (const void *) line_buffer[term], save_buffer_idx[term]);
        return save_buffer_idx[term];
    }
    // sti();
    // should check for ENTER and BACKSPACE here
    return -1;
}

/* terminal_write
 * Inputs: fd, buf, nbytes
 * Return Value: num bytes wrote
 * Function: prints the chars in input buf to screen */
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes) {
    int i;
    int term;
    if(!is_terminals_initialized())
    {
        term = terminal_idx;
    }
    else
    {
        term = get_schedule_idx();
    }
    
    for (i = 0; i < nbytes; i++) {
        if(((char*)buf)[i] == '\t')
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
int get_saved_screen_x(int term)
{
    return save_screen_x[term];
}

int get_saved_screen_y(int term)
{
    return save_screen_y[term];
}

void set_saved_screen_x(int term, int x)
{
    save_screen_x[term] = x;
}

void set_saved_screen_y(int term, int y)
{
    save_screen_y[term] = y;
}

void terminal_switch (int t_idx)
{
    // if (!temp_terminal_flag) return;
    if(t_idx > 2 || t_idx < 0 || new_terminal_flag == 1) return;
    // if(terminal_pids[t_idx] == -1 && !is_pcb_available()) return;
    // save_screen_x[terminal_idx] = get_screen_x();
    // save_screen_y[terminal_idx] = get_screen_y();

    video_memory_page_table[VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx].base_31_12 = VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx;
    memcpy((void *) (VIDEO + FOUR_KB * (terminal_idx + 1)), (void *) VIDEO, 4000);
    
    terminal_idx = t_idx;
    
    memcpy((void *) VIDEO, (void *) (VIDEO + FOUR_KB * (terminal_idx + 1)), 4000);
    video_memory_page_table[VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx].base_31_12 = VIDEO_ADDRESS / FOUR_KB;
    flush_tlb();
    
    //screen save stuff
    // video_memory_page_table[VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx].base_31_12 = VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx;
    // flush_tlb();
    // if(get_child_pcb(terminal_idx)->screen_flag_set == 1) 
    //     *(get_child_pcb(terminal_idx)->user_screen_start) = VIDEO + FOUR_KB * (terminal_idx + 1);
    // memcpy((void *) (VIDEO + FOUR_KB * (terminal_idx + 1)), (void *) VIDEO, 4096);
    // terminal_idx = t_idx;
    
    // // video_memory_page_table[VIDEO_ADDRESS / FOUR_KB].base_31_12 = VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx;
    // memcpy((void *) VIDEO, (void *) (VIDEO + FOUR_KB * (terminal_idx + 1)), 4096);
    // video_memory_page_table[VIDEO_ADDRESS / FOUR_KB + 1 + terminal_idx].base_31_12 = VIDEO_ADDRESS / FOUR_KB;
    // flush_tlb();
    
    // set_vid_mem(terminal_idx, terminal_idx);
    
    set_vid_mem(terminal_idx);

    // if (terminal_pids[terminal_idx] == -1) {
    //     clear_terminal(terminal_idx);
    //     new_terminal_flag = 1; // need to set up new terminal
    // }

    // send_eoi(1);
    // sti();
}

void init_terminals_vidmaps()
{
    // 8kb to 20kb is terminal vmem
    // set_vid_mem(0, 0);
    // video_memory_page_table[    (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = 1 + (VIDEO_ADDRESS / FOUR_KB);
    video_memory_page_table[1 + (VIDEO_ADDRESS / FOUR_KB)].p = 1; 
    video_memory_page_table[1 + (VIDEO_ADDRESS / FOUR_KB)].us = 1;
    video_memory_page_table[1 + (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = VIDEO/FOUR_KB; //1 + (VIDEO_ADDRESS / FOUR_KB);
    video_memory_page_table[2 + (VIDEO_ADDRESS / FOUR_KB)].p = 1; 
    video_memory_page_table[2 + (VIDEO_ADDRESS / FOUR_KB)].us = 1;
    video_memory_page_table[2 + (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = 2 + (VIDEO_ADDRESS / FOUR_KB);
    video_memory_page_table[3 + (VIDEO_ADDRESS / FOUR_KB)].p = 1; 
    video_memory_page_table[3 + (VIDEO_ADDRESS / FOUR_KB)].us = 1;
    video_memory_page_table[3 + (VIDEO_ADDRESS / FOUR_KB)].base_31_12 = 3 + (VIDEO_ADDRESS / FOUR_KB);
    flush_tlb();
}

int get_terminal_arr(int index) {
    if (index < 0 || index > 2) return -1;
    return terminal_pids[index];
}

void set_terminal_arr(int index, int val) {
    terminal_pids[index] = val;
}
