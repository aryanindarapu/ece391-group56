#include "system_call.h"
#include "paging.h"

int system_call(){
    system_handler();
}

/* NO NEED TO IMPLEMENT YET(CHECKPOINT 3.2 COMMENT) */
// uint32_t halt (uint8_t status){

// }
// uint32_t execute (const uint8_t* command){

// }

/* TODO: write the read system call */
uint32_t read (uint32_t fd, void* buf, uint32_t nbytes){

}

/* TODO: write the write system call */
uint32_t write (uint32_t fd, const void* buf, uint32_t nbytes){

}

/* TODO: write the open system call */
uint32_t open (const uint8_t* filename){

}

/* TODO: write the close system call */
uint32_t close (uint32_t fd){

}

/* NO NEED TO IMPLEMENT YET (CHECKPOINT 3.2 COMMENT)*/
// uint32_t getargs (uint8_t* buf, uint32_t nbytes){

// }
// uint32_t vidmap (uint8_t** screen_start){

// }

/* EXTRA CREDIT SYSTEM CALLS */
// uint32_t set_handler (uint32_t signum, void* handler_address){

// }
// uint32_t sigreturn (void){

// }