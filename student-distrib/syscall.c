#include "syscall.h"
#include "paging.h"
#include "file_system_driver.h"
#include "lib.h"

int system_call() {
    // system_handler();
    return 0;
}

/* NO NEED TO IMPLEMENT YET(CHECKPOINT 3.2 COMMENT) */
// uint32_t halt (uint8_t status){

// }
// uint32_t execute (const uint8_t* command){

// }

/* TODO: write the read system call */
int32_t read (uint32_t fd, void* buf, uint32_t nbytes) {
    if (file_desc_arr[fd].flags == 0) return -1;

    file_desc_t file_desc = file_desc_arr[fd];

    read_data(file_desc.inode, file_desc.file_pos, (uint8_t *) buf, nbytes);
    file_desc.file_pos++;

    return 0;
}

/* TODO: write the write system call */
int32_t write (uint32_t fd, const void* buf, uint32_t nbytes) {
    //we dont need to implement for this checkpoint
    return -1;
}

/* TODO: write the open system call */
int32_t open (const uint8_t* filename) {
    dentry_t file_dentry;
    int file_desc_index;
    
    //ensure the filename is valid
    if (filename == NULL){
        return -1;
    }

    // ensure the file desc has space AND find the index to emplace this file
    for (file_desc_index = 0; file_desc_index < MAX_FILE_DESC; file_desc_index++) {
        //is the index empty?
        if (file_desc_arr[file_desc_index].flags == 0) {
            break; //this file_desc_index is the one we will emplace the file to 
        }
    }

    // Ensure there was space left
    if (file_desc_index >= MAX_FILE_DESC) {
        return -1;
    }

    /* Let read_dentry_by_name populate our dentry, or tell us that the dentry doesn't exist in our filesystem */
    if (read_dentry_by_name (filename, &file_dentry) == -1) { 
        return -1; //file doesn't exist
    }

    /* The filename exists, the file_descriptor has space, and the dentry has been populated
        by the filesystem function read_dentry_by_name */
    /* So set all of the fields */
    switch (file_dentry.file_type) {
        case 0: // rtc driver
            // file_desc_arr[file_desc_index].file_ops_ptr = rtc_op;
            // TODO: FILL OUT THE REST OF THE FIELDS FOR ALL CASES 
            // file_desc_arr[file_desc_index].file_pos = idk
            // file_desc_arr[file_desc_index].flags = idk
            // opcode 0 specifies the opening operation
            // rtc_op(0);
            break;
        case 1: //dir
            // file_desc_arr[file_desc_index].file_ops_ptr = dir_op;
            // dir_op(0);
            break;
        case 2: //file
            // file_desc_arr[file_desc_index].file_ops_ptr = file_op;
            // file_op(0);
            break;
        default:
            break;
    }

    file_desc_arr[file_desc_index].inode = file_dentry.inode_num;
    file_desc_arr[file_desc_index].flags = 1;
    file_desc_arr[file_desc_index].file_pos = 0;

    // check dentry file type and add to file descriptor array

    // file_desc_index++;
    //TODO we just do this now right?
    /* this fd index is now taken */

    return 0;
}

/* TODO: write the close system call */
int32_t close (uint32_t fd) {
    return 0;
    // TODO: ENSURE THAT THIS WORKS 
    // close based on the file types functions, close is opcode 1
    // typedef int func(void);
    // func* op_ptr = file_desc_arr[fd].file_ops_ptr;
    // op_ptr(1); //close the file using this function
    
    // /* clear the fd element data */
    // file_desc_arr[fd].file_ops_ptr = NULL;
    // file_desc_arr[fd].inode = 0;
    // file_desc_arr[fd].file_pos = 0;
    // file_desc_arr[fd].flags = 0;

    // /* turn off the present flag for the fd element */
    // file_desc_arr[fd].flags = 0;


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
