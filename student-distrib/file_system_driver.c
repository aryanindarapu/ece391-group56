#include "file_system_driver.h"
#include "lib.h"
#include "types.h"

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
}

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

/* 
 * file_open
 *   DESCRIPTION: Opens a file given a file name
 *   INPUTS: fname - file name to open
 *   OUTPUTS: none
 *   RETURN VALUE: fd index on success, -1 if failure
 *   SIDE EFFECTS: adds to file descriptor array
 */
int32_t file_open(const uint8_t * filename) {
    dentry_t file_dentry;
    int file_desc_index;
    
    // ensure the filename is valid
    if (filename == NULL) {
        return -1;
    }

    // ensure the file desc has space AND find the index to emplace this file
    for (file_desc_index = 2; file_desc_index < MAX_FILE_DESC; file_desc_index++) {
        //is the index empty?
        if (file_desc_arr[file_desc_index].flags == 0) {
            //https://stackoverflow.com/questions/9932212/jump-table-examples-in-c
            break; // this file_desc_index is the one we will emplace the file to 
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
    
    /* this fd index is now taken */
    file_desc_arr[file_desc_index].inode = file_dentry.inode_num;
    file_desc_arr[file_desc_index].flags = 1;
    file_desc_arr[file_desc_index].file_pos = 0;

    return file_desc_index;
}

/* 
 * file_close
 *   DESCRIPTION: Closes a file given a file descriptor
 *   INPUTS: fd - file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: removes from file descriptor array
 */
int32_t file_close(uint32_t fd) {
    // make the file unreadable and remove its pointers to operation functions
    file_desc_arr[fd].flags = 0;
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
int32_t file_read(uint32_t fd, void* buf, uint32_t nbytes) {
    if (file_desc_arr[fd].flags == 0) return -1;

    file_desc_t file_desc = file_desc_arr[fd];
    int32_t status = read_data(file_desc.inode, file_desc.file_pos, (uint8_t *) buf, nbytes);

    if (status == -1) return -1;
    file_desc.file_pos += status;

    return status;
}

int32_t file_write(uint32_t fd, const void* buf, uint32_t nbytes) {
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
    int file_desc_index;
    dentry_t dir_dentry;

    // ensure the file desc has space AND find the index to emplace this file
    for (file_desc_index = 2; file_desc_index < MAX_FILE_DESC; file_desc_index++) {
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
    if (read_dentry_by_name (filename, &dir_dentry) == -1) { 
        return -1; //file doesn't exist
    }

    file_desc_arr[file_desc_index].flags = 1;
    file_desc_arr[file_desc_index].inode = dir_dentry.inode_num;
    file_desc_arr[file_desc_index].file_pos = 0;

    return file_desc_index;
}

/* 
 * dir_close
 *   DESCRIPTION: Closes a directory given a file descriptor
 *   INPUTS: fd - file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: removes from file descriptor array
 */
int32_t dir_close(uint32_t fd) {
    //clear the flag and remove the file operations pointers for this process
    file_desc_arr[fd].flags = 0;
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
int32_t dir_read(uint32_t fd, void* buf, uint32_t nbytes) {
    /* Directory read is really what separates file from dir operations 
        - We need to iterate through the existing dentrys and return them
          the buffer (ls)
    */
   /* 
        Note that we don't care what fd or nbytes are: we already know that dir_read got called which means
        syscall operations table is set to the dir_ops_ptr for this entry, as for nbytes we need to display 
        all of the entry names anyway we nbytes can't be specified by the caller
    */
    if (file_desc_arr[fd].file_pos == boot_block_ptr->num_dirs) return 0;

   char buffer[80];
   char temp[10];
   char filename[11] = {'f', 'i', 'l', 'e', '_', 'n', 'a', 'm', 'e', ':', ' '};
   char filetype[11] = {'f', 'i', 'l', 'e', '_', 't', 'y', 'p', 'e', ':', ' '};
   char filesize[11] = {'f', 'i', 'l', 'e', '_', 's', 'i', 'z', 'e', ':', ' '};
   char cur_filename[FILENAME_SIZE];
   int i;
   int32_t cur_size;
    for(i = 0; i < 80; i++) buffer[i] = ' ';
   // ensure the fd is a valid entry
   if (file_desc_arr[fd].flags == 0) return -1;

    /* fill out the buffer based given the number of dentrys from boot_block */

    /* do 32 character name emplacement */
    dentry_t cur_file = boot_block_ptr->dir_entries[file_desc_arr[fd].file_pos];
    strcpy(cur_filename, cur_file.file_name);
    if (strlen(cur_filename) > 32){
        for(i = 32; i < strlen(cur_filename); i++){
            cur_filename[i] = ' ';
        }
        cur_filename[32] = '\0';
    }
    //garbage checking routine
    i = 0;
    while(cur_filename[i] != '\0'){
        i++;
    }
    for(i = i; i < FILENAME_SIZE; i++){
        cur_filename[i] = ' ';
    }
    ///////////////////////////////////
    for (i = 0; i < FILENAME_SIZE; i++){
        //cur_filename has many trailing '\0' so we need to make them spaces
        buffer[FORMATTER_LENGTH + i] = (cur_filename[i] != '\0') ? cur_filename[i] : ' ';
    }
    
    /* do file type emplacement */
    buffer[FORMATTER_LENGTH * 2 + FILENAME_SIZE + 2] = (char)(cur_file.file_type + 48);
    
    /* do file size emplacement */
    cur_size = inode_ptr[cur_file.inode_num].length;
    //printf("%d", cur_size);
    itoa(cur_size, temp, 10);
    for(i = 0; i < 9; i++) buffer[FORMATTER_LENGTH * 3 + FILENAME_SIZE + 5 + i] = ' ';
    for (i = 0; i < strlen(temp) ; i++){
        //printf("\n %d \n", (int)temp[i]);
        buffer[FORMATTER_LENGTH * 3 + FILENAME_SIZE + 5 + i] = (temp[i] >= 48 && temp[i] <= 57) ? temp[i] : ' ';
    }

     /* setup filename formatting */
    for(i = 0; i < FORMATTER_LENGTH; i++){
        buffer[i] = filename[i];
    }

    /* setup filetype formatting */
    buffer[FORMATTER_LENGTH + FILENAME_SIZE] = ',';
    buffer[FORMATTER_LENGTH + FILENAME_SIZE + 1] = ' ';
    for(i = 0; i < FORMATTER_LENGTH; i++){
            buffer[FORMATTER_LENGTH + FILENAME_SIZE + 2 + i] = filetype[i];
    }

    /* setup filesize formatting */
    buffer[FORMATTER_LENGTH * 2 + FILENAME_SIZE + 3] = ',';
    buffer[FORMATTER_LENGTH * 2 + FILENAME_SIZE + 4] = ' ';
    for(i = 0; i < FORMATTER_LENGTH; i++){
        buffer[FORMATTER_LENGTH * 2 + FILENAME_SIZE + 5 + i] = filesize[i];
    }

    /* buffer is now prepared to send to buf */
    buffer[79] = '\n';
    memcpy(buf,(void *)buffer, 80 );
    //buf = (void *)buffer;
    file_desc_arr[fd].file_pos++;

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
int32_t dir_write(uint32_t fd, const void* buf, uint32_t nbytes) {
    return -1;
}
