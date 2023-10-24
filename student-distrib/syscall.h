#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "types.h"

#ifndef ASM
int system_call();

// extern void system_handler();

/* opcodes at the moment are */
/*
    open : 0
    close : 1
    read : 2
    write : 3
*/
// extern void rtc_op(int opcode);
// extern void dir_op(int opcode);
// extern void file_op(int opcode);

/* DONT NEED TO IMPLEMENT YET */
// int32_t halt (uint8_t status);
// int32_t execute (const uint8_t* command);

int32_t open (const uint8_t* filename);
int32_t close (uint32_t fd);
int32_t read (uint32_t fd, void* buf, uint32_t nbytes);
int32_t write (uint32_t fd, const void* buf, uint32_t nbytes);


/* DONT NEED TO IMPLEMENT YET */
// int32_t getargs (uint8_t* buf, uint32_t nbytes);
// int32_t vidmap (uint8_t** screen_start);

/* BOTH OF THESE SYSTEM_CALLS ARE EXTRA CREDIT TO IMPLEMENT */
// int32_t set_handler (uint32_t signum, void* handler_address);
// int32_t sigreturn (void);


#endif /* ASM */

#endif /* _SYSTEM_CALL_H */
