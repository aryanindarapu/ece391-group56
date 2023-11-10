#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "paging.h"
#include "file_system_driver.h"

#define EIGHT_MB 0x800000
#define FOUR_MB 0x400000
#define EIGHT_KB 0x2000
#define USER_MEM_VIRTUAL_ADDR 0x8000000 // 128 MB
#define PROGRAM_START 0x08048000
#define EIP_START 24
#define STACK_FENCE_SIZE 4
#define PHYSICAL_MEMORY 2

#define MAGIC_BYTE_0 0x7F
#define MAGIC_BYTE_1 0x45
#define MAGIC_BYTE_2 0x4C
#define MAGIC_BYTE_3 0x46

#define EXCEPTION_OCCURRED_VAL 256

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

#endif /* ASM */

#endif /* _SYSTEM_CALL_H */
