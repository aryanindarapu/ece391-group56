#include "syscall.h"


int32_t execute (const uint8_t* command){
    // Do iret to go to user memory
    // start program at given value in user memory
    int pid;
    for (pid = 0; pid < MAX_NUM_PROCESSES; pid++) {
        if (process_flags[pid] == 0) {
            break;
        }
    }

    if (pid >= MAX_NUM_PROCESSES) {
        return -1;
    }

    pcb_t * pcb = (pcb_t *)(EIGHT_MB - (pid + 1) * EIGHT_KB);
    // TODO: check commands here
}

int32_t open (const uint8_t* filename) {
    dentry_t file_dentry;
    uint32_t fd;
    
    // ensure the filename is valid
    if (filename == NULL) {
        return -1;
    }

    // ensure the file desc has space AND find the index to emplace this file
    for (fd = 2; fd < MAX_FILE_DESC; fd++) {
        // is the index empty?
        if (file_desc_arr[fd].flags == 0) {
            break; //this fd is the one we will emplace the file to 
        }
    }

    // Ensure there was space left
    if (fd >= MAX_FILE_DESC) {
        return -1;
    }

    /* The filename exists, the file_descriptor has space, and the dentry has been populated
       by the filesystem function read_dentry_by_name */
    /* So set all of the fields */


    switch (file_dentry.file_type) {
        case 0: // rtc driver
            fd = rtc_open(filename);
            if (fd < 0) return -1;
            // TODO: need to make rtc_ops_table
            // file_desc_arr[fd].ops_ptr = rtc_ops_table;
            return 0;
        case 1: // dir
            fd = dir_open(filename);
            if (fd < 0) return -1;
            file_desc_arr[fd].ops_ptr = dir_ops_table;
            break;
        case 2: // file
            fd = file_open(filename);
            if (fd < 0) return -1;
            file_desc_arr[fd].ops_ptr = file_ops_table;
            break;
        default:
            return -1;
    }

    return fd;
}

int32_t close (uint32_t fd) {
    if (fd <= 1 || fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    if (!file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return file_desc_arr[fd].ops_ptr.close(fd);
}

int32_t read (uint32_t fd, void* buf, uint32_t nbytes) {
    if (fd <= 1 || fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    if (!file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return file_desc_arr[fd].ops_ptr.read(fd, buf, nbytes);
}

int32_t write (uint32_t fd, const void* buf, uint32_t nbytes) {
    if (fd <= 1 || fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    if (!file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return file_desc_arr[fd].ops_ptr.write(fd, buf, nbytes);
}

// TODO: add init system calls the sets up stdin and stdout file entries

/* NO NEED TO IMPLEMENT YET(CHECKPOINT 3.2 COMMENT) */
// int32_t halt (uint8_t status){

// }



/* NO NEED TO IMPLEMENT YET (CHECKPOINT 3.2 COMMENT)*/
// uint32_t getargs (uint8_t* buf, uint32_t nbytes){

// }

// uint32_t vidmap (uint8_t** screen_start){

// }

/* EXTRA CREDIT SYSTEM CALLS */
int32_t set_handler (uint32_t signum, void* handler_address){
    return -1;
}

int32_t sigreturn (void){
    return -1;
}
