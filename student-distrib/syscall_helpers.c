#include "syscall_helpers.h"

/* 
 * read_dentry_by_name
 *   DESCRIPTION: Goes through the dentries and compares the 
 *   INPUTS: fname - file name to search for
 *           dentry - dentry to populate with file information
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: none
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry) {
    uint32_t dir_index;

    // Scan through directory entries to find file name
    for (dir_index = 0; dir_index < boot_block_ptr->num_dirs; dir_index++) {
        if (strlen((const int8_t *) fname) == strlen((const int8_t *) boot_block_ptr->dir_entries[dir_index].file_name) 
            && strncmp((const int8_t *) fname, (const int8_t *) boot_block_ptr->dir_entries[dir_index].file_name, FILENAME_SIZE) == 0) {
            // File found in our boot block so we update our dentry
            strcpy(dentry->file_name, (const int8_t *) fname);
            // Now we update the dentry with the inode and type
            return read_dentry_by_index(dir_index, dentry);
        }
    }
    
    // File not found
    return -1;

}

/* 
 * read_dentry_by_index
 *   DESCRIPTION: Goes through the dentries and compares the 
 *   INPUTS: index - index of the directory entry
 *           dentry - dentry to populate with file information
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: none
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry) {
    // Open up a file and set up the file object
    // NOTE: index is the directory index
    if (index >= boot_block_ptr->num_inodes) {
        return -1;
    }
    
    dentry_t found_dentry = boot_block_ptr->dir_entries[index];
    
    dentry->file_type = found_dentry.file_type;
    dentry->inode_num = found_dentry.inode_num;

    return 0;
}

/* 
 * read_data
 *   DESCRIPTION: Reads data from a file given an inode and offsetm
 *                and writes length bytes of data into the given buffer.
 *   INPUTS: inode - inode number of the file
 *           offset - offset of the file
 *           buf - buffer to read into
 *           length - length of the file
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: none
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    // Check if inode is valid
    if (inode >= boot_block_ptr->num_inodes) {
        return -1;
    }

    inode_t * curr_inode = inode_ptr + inode;
    int curr_byte_idx;
    int data_block_idx;
    int within_data_block_idx;
    for (curr_byte_idx = offset; curr_byte_idx < length + offset; curr_byte_idx++) {
        // reached end of file
        if (curr_byte_idx >= curr_inode->length) {
            return 0;
        }

        // Get indices for data block and within data block
        data_block_idx = curr_byte_idx / DATA_BLOCK_SIZE;
        within_data_block_idx = curr_byte_idx % DATA_BLOCK_SIZE;
        uint32_t data_block_num = curr_inode->data_blocks[data_block_idx];

        // bad data block
        if (data_block_num >= boot_block_ptr->num_data_blocks) {
            return -1;
        }

        data_block_t * data_block = data_block_ptr + data_block_num;
        buf[curr_byte_idx - offset] = data_block->data[within_data_block_idx];
    }

    return length;
}

// TODO BEN: comment
pcb_t * get_pcb_ptr() {
    // TODO: should this actually be ANDing ESP bitmask?
    // https://www.cs.columbia.edu/~junfeng/10sp-w4118/lectures/l07-proc-linux.pdf
    pcb_t * pcb_ptr;
    // AND ESP WITH PCB_BITMASK
    asm volatile (
        "movl %%esp, %%eax      ;\
         andl $0xFFFFE000, %%eax   ;\
        "
        : "=a" (pcb_ptr)
    );
    return pcb_ptr;
}
