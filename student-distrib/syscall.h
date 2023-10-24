#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "types.h"
#include "paging.h"
#include "file_system_driver.h"
#include "lib.h"

#define MAX_NUM_PROCESSES 6
#define EIGHT_MB 0x800000
#define EIGHT_KB 0x2000
#define PAGE_BASE_ADDR 0x08000000

#ifndef ASM

extern void system_call_handler();

/* DONT NEED TO IMPLEMENT YET */
// int32_t halt (uint8_t status);
// int32_t execute (const uint8_t* command);

int32_t open (const uint8_t* filename);
int32_t close (uint32_t fd);
int32_t read (uint32_t fd, void* buf, uint32_t nbytes);
int32_t write (uint32_t fd, const void* buf, uint32_t nbytes);

typedef struct pcb {
    file_desc_t file_desc_arr[MAX_FILE_DESC];
    uint32_t esp_reg;
    pcb_t * child_pcb_ptr;
} pcb_t;


/* DONT NEED TO IMPLEMENT YET */
// int32_t getargs (uint8_t* buf, uint32_t nbytes);
// int32_t vidmap (uint8_t** screen_start);

/* BOTH OF THESE SYSTEM_CALLS ARE EXTRA CREDIT TO IMPLEMENT */
// int32_t set_handler (uint32_t signum, void* handler_address);
// int32_t sigreturn (void);

uint8_t process_flags[MAX_NUM_PROCESSES];

#endif /* ASM */

#endif /* _SYSTEM_CALL_H */
