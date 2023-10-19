#include "syscall.h"
#include "paging.h"
#include "file_system_driver.h"

int system_call() {
    system_handler();
}

/* NO NEED TO IMPLEMENT YET(CHECKPOINT 3.2 COMMENT) */
// uint32_t halt (uint8_t status){

// }
// uint32_t execute (const uint8_t* command){

// }

/* TODO: write the read system call */
uint32_t read (uint32_t fd, void* buf, uint32_t nbytes) {
    if (fd > file_desc_index) return -1;

    file_desc_t file_desc = file_desc_arr[fd];

    // TODO: File descriptor is directory or RTC entry
    if (file_desc.inode == 0) {

    }

    read_data(file_desc.inode, file_desc.file_pos, (uint8_t *) buf, nbytes);
    file_desc.file_pos++;
}

/* TODO: write the write system call */
uint32_t write (uint32_t fd, const void* buf, uint32_t nbytes) {
    //we dont need to implement for this checkpoint
    return -1;
}

/* TODO: write the open system call */
uint32_t open (const uint8_t* filename) {
    dentry_t file_dentry;
    int i;
    
    // if there is no file space remaining in the file decriptor 
    if (file_desc_index > 7) {
        return -1;
    }

    /* find the inode index this file should populate */
    /* we dont know the file_type so we let read_dentry_by_name implement it since
        the function is going to add the filename on the file directory */
    if (read_dentry_by_name (filename, &file_dentry) == -1) { //if the file doesn't exist == -1
        return -1;
    }

    // check dentry file type and add to file descriptor array

    file_desc_index++;

    return 0;
}

/* TODO: write the close system call */
uint32_t close (uint32_t fd) {
    file_desc_index--;
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