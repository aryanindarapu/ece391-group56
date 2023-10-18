#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#ifndef ASM
int system_call();

extern void system_handler();

/* DONT NEED TO IMPLEMENT YET */
// uint32_t halt (uint8_t status);
// uint32_t execute (const uint8_t* command);

uint32_t read (uint32_t fd, void* buf, uint32_t nbytes);
uint32_t write (uint32_t fd, const void* buf, uint32_t nbytes);
uint32_t open (const uint8_t* filename);
uint32_t close (uint32_t fd);

/* DONT NEED TO IMPLEMENT YET */
// uint32_t getargs (uint8_t* buf, uint32_t nbytes);
// uint32_t vidmap (uint8_t** screen_start);

/* BOTH OF THESE SYSTEM_CALLS ARE EXTRA CREDIT TO IMPLEMENT */
// uint32_t set_handler (uint32_t signum, void* handler_address);
// uint32_t sigreturn (void);


#endif /* ASM */

#endif /* _SYSTEM_CALL_H */