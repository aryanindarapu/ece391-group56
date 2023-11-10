#include "file_system_driver.h"
#include "lib.h"
#include "types.h"
#include "syscall.h"
#include "syscall_helpers.h"
#include "process.h"

/* 
 * init_file_system
 *   DESCRIPTION: Initializes the file system pointers to point to memory
 *                addresses based on boot block.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initializes inode_ptr and data_block_ptr
 */
void init_file_system(void) {
    // Boot block pointer set in kernel.c
    inode_ptr = (inode_t *)(boot_block_ptr + 1); // increase by size of pointer 
    data_block_ptr = (data_block_t *)(inode_ptr + boot_block_ptr->num_inodes);
    init_ops_tables();
}

/* 
 * file_open
 *   DESCRIPTION: Opens a file given a file name
 *   INPUTS: fname - file name to open
 *   OUTPUTS: none
 *   RETURN VALUE: fd index on success, -1 if failure
 *   SIDE EFFECTS: adds to file descriptor array
 */
int32_t file_open(const uint8_t * filename) {
    return 0;
}

/* 
 * file_close
 *   DESCRIPTION: Closes a file given a file descriptor
 *   INPUTS: fd - file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: removes from file descriptor array
 */
int32_t file_close(int32_t fd) {
    pcb_t * pcb = get_curr_pcb_ptr();
    // make the file unreadable and remove its pointers to operation functions
    pcb->file_desc_arr[fd].flags = 0;
    return 0;
}

/* 
 * file_read
 *   DESCRIPTION: Reads a file given a file descriptor
 *   INPUTS: fd - file descriptor
 *           buf - buffer to read into
 *           nbytes - number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: none
 */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes) {
    pcb_t * pcb = get_curr_pcb_ptr();

    if (pcb->file_desc_arr[fd].flags == 0) return -1;

    file_desc_t file_desc = pcb->file_desc_arr[fd];
    int32_t status = read_data(file_desc.inode, file_desc.file_pos, (uint8_t *) buf, nbytes);

    if (status == -1) return -1;
    pcb->file_desc_arr[fd].file_pos += status;

    return status;
}

int32_t file_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/* 
 * dir_open
 *   DESCRIPTION: Opens a directory given a file name
 *   INPUTS: fname - file name to open
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: adds to file descriptor array
 */
int32_t dir_open(const uint8_t * filename) {
    return 0;
}

/* 
 * dir_close
 *   DESCRIPTION: Closes a directory given a file descriptor
 *   INPUTS: fd - file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: removes from file descriptor array
 */
int32_t dir_close(int32_t fd) {
    //clear the flag and remove the file operations pointers for this process
    pcb_t * pcb = get_curr_pcb_ptr();

    pcb->file_desc_arr[fd].flags = 0;
    return 0;
}

/* 
 * dir_read
 *   DESCRIPTION: Reads a directory given a file descriptor
 *   INPUTS: fd - file descriptor
 *           buf - buffer to read into
 *           nbytes - number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: none
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes) {
    /* Directory read is really what separates file from dir operations 
        - We need to iterate through the existing dentrys and return them
          the buffer (ls)
    */
    /* 
        Note that we don't care what fd or nbytes are: we already know that dir_read got called which means
        syscall operations table is set to the dir_ops_ptr for this entry, as for nbytes we need to display 
        all of the entry names anyway we nbytes can't be specified by the caller
    */
    pcb_t * pcb = get_curr_pcb_ptr();
    if (pcb->file_desc_arr[fd].file_pos >= boot_block_ptr->num_dirs) {
        return 0;
    }

    // ensure the fd is a valid entry
    if (pcb->file_desc_arr[fd].flags == 0) return -1;

    /* fill out the buffer based given the number of dentrys from boot_block */
    dentry_t cur_file = boot_block_ptr->dir_entries[pcb->file_desc_arr[fd].file_pos];
    if (nbytes > FILENAME_SIZE) return -1;
    int i;
    for (i = 0; i < nbytes; i++) {
        ((char *) buf)[i] = cur_file.file_name[i];
    }
    pcb->file_desc_arr[fd].file_pos++; // increments current file index
    return nbytes;
}

/* 
 * dir_write
 *   DESCRIPTION: Writes a directory to the given buffer.
 *   INPUTS: fd - file descriptor
 *           buf - buffer to read into
 *           nbytes - number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: none
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/* 
 * empty_open
 *   DESCRIPTION: Placeholder for open function
 *   INPUTS: fname - file name to open
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t empty_open(const uint8_t * filename) {
    return -1;
}

/* 
 * empty_close
 *   DESCRIPTION: Placeholder for close function
 *   INPUTS: fd - file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t empty_close(int32_t fd) {
    return -1;
}

/* 
 * empty_read
 *   DESCRIPTION: Placeholder for read function
 *   INPUTS: fd - file descriptor
 *           buf - buffer to read into
 *           nbytes - number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t empty_read(int32_t fd, void* buf, int32_t nbytes) {
    return -1;
}

/* 
 * empty_write
 *   DESCRIPTION: Placeholder for write function
 *   INPUTS: fd - file descriptor
 *           buf - buffer to write from
 *           nbytes - number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t empty_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/* 
 * init_ops_tables
 *   DESCRIPTION: Initializes the operations tables for each file type
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initializes operations tables
 */
void init_ops_tables() {
    dir_ops_table.open = dir_open;
    dir_ops_table.close = dir_close;
    dir_ops_table.read = dir_read;
    dir_ops_table.write = dir_write;

    stdin_ops_table.open = empty_open;
    stdin_ops_table.close = empty_close;
    stdin_ops_table.read = terminal_read;
    stdin_ops_table.write = empty_write;
    
    stdout_ops_table.open = empty_open;
    stdout_ops_table.close = empty_close;
    stdout_ops_table.read = empty_read;
    stdout_ops_table.write = terminal_write;
    
    file_ops_table.open = file_open;
    file_ops_table.close = file_close;
    file_ops_table.read = file_read;
    file_ops_table.write = file_write;

    rtc_ops_table.open = rtc_open;
    rtc_ops_table.close = rtc_close;
    rtc_ops_table.read = rtc_read;
    rtc_ops_table.write = rtc_write;
}
