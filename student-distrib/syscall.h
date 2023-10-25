#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "paging.h"
#include "file_system_driver.h"

#define MAX_NUM_PROGRAMS 6
#define EIGHT_MB 0x800000
#define FOUR_MB 0x400000
#define EIGHT_KB 0x2000
#define USER_MEM_VIRTUAL_ADDR 0x08000000 // 128 MB
#define EIP_START 24

#define MAGIC_BYTE_0 0x7F
#define MAGIC_BYTE_1 0x45
#define MAGIC_BYTE_2 0x4C
#define MAGIC_BYTE_3 0x46

#ifndef ASM

extern void system_call_handler();

/* DONT NEED TO IMPLEMENT YET */
int32_t halt (uint8_t status);
int32_t execute (const uint8_t* command);

int32_t open (const uint8_t* filename);
int32_t close (uint32_t fd);
int32_t read (uint32_t fd, void* buf, uint32_t nbytes);
int32_t write (uint32_t fd, const void* buf, uint32_t nbytes);

/* DONT NEED TO IMPLEMENT YET */
int32_t getargs (uint8_t* buf, uint32_t nbytes);
int32_t vidmap (uint8_t** screen_start);

/* BOTH OF THESE SYSTEM_CALLS ARE EXTRA CREDIT TO IMPLEMENT */
int32_t set_handler (uint32_t signum, void* handler_address);
int32_t sigreturn (void);

typedef struct pcb {
    uint32_t pid; 
    uint32_t esp_reg;
    uint32_t eip_reg;
    uint8_t * commands;
    pcb_t * parent_pcb_ptr;
    file_desc_t file_desc_arr[MAX_FILE_DESC];
} pcb_t;

int pid = 0;

uint8_t process_flags[MAX_NUM_PROGRAMS];

#endif /* ASM */

#endif /* _SYSTEM_CALL_H */
