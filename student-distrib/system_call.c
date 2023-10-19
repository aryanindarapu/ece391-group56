#include "system_call.h"
#include "paging.h"
#include "file_system.h"

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
    /* 
    create the dentry object to pass to read_dentry_by_name
    manually fill the file name field in the dentry object 
    */
    for (i = 0; i < strlen(filename); i++) {
        file_dentry.file_name[i] = filename[i];
    }
    /* find the inode index this file should populate */
    /* we dont know the file_type so we let read_dentry_by_name implement it since
        the function is going to add the filename on the file directory */
    if (read_dentry_by_name (filename, &file_dentry) == -1){ //if the file doesn't exist == -1
        file_sys.boot_block.inode_count++;
        file_sys.boot_block.dir_count++;
    }

    file_desc_index++;

    return 0;
}

// typedef struct boot_block_t {
//     int32_t dir_count; // # of entries in our directory
//     int32_t inode_count; // # of inodes
//     int32_t data_count; // # of total data blocks
//     uint8_t reserved[52]; // reserved 52 bytes
//     dentry_t dir_entries[63]; // list of dir. entries (63 so total size of boot is 4kB)
// } boot_block_t;

// typedef struct dentry_t {
//     char file_name[FILENAME_SIZE]; // 32B for the file name so 32 8-bit values
//     int32_t file_type; // Will be 0 for RTC, 1 for dir., and 2 for file
//     int32_t inode_num; // Which inode holds the data block information
//     int8_t reserved[24]; // reserved
// } dentry_t;

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