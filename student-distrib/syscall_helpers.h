#ifndef _SYSCALL_HELPERS_H
#define _SYSCALL_HELPERS_H

#include "types.h"
#include "file_system_driver.h"
#include "syscall.h"

#define PCB_BITMASK 0xFFFFE000
#define MAX_NUM_PROGRAMS 6

typedef struct pcb {
    int32_t pid; 
    int32_t parent_pid;
    int32_t child_pid;
    uint32_t kernel_ebp;
    uint32_t kernel_esp;
    uint32_t base_ebp;
    uint32_t base_esp;
    uint32_t user_esp;
    uint32_t user_eip;
    uint8_t commands[LINE_BUFFER_SIZE];
    file_desc_t file_desc_arr[MAX_FILE_DESC];
} pcb_t;

uint32_t pcb_flags[MAX_NUM_PROGRAMS];

/* file system helper functions */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

pcb_t * get_curr_pcb_ptr (void);
pcb_t * get_pcb_ptr(int32_t pid);
pcb_t * get_child_pcb(int32_t terminal_num);
int is_pcb_available();

void setup_user_page(uint32_t table_addr);

#endif
