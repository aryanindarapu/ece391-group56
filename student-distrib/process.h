#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"
#include "file_system_driver.h"

#define MAX_NUM_PROGRAMS 6

int32_t process_switch(int32_t terminal_num);
int32_t process_vidmap_change(int32_t terminal_from, int32_t terminal_to);

typedef struct pcb {
    int32_t pid; 
    int32_t parent_pid;
    int32_t child_pid;
    uint32_t kernel_ebp;
    uint32_t kernel_esp;
    uint32_t user_esp;
    uint32_t user_eip;
    uint8_t commands[LINE_BUFFER_SIZE];
    uint32_t return_addr;
    file_desc_t file_desc_arr[MAX_FILE_DESC];
    // uint8_t old_commands[LINE_BUFFER_SIZE];
} pcb_t;

uint32_t pcb_flags[MAX_NUM_PROGRAMS];

#endif