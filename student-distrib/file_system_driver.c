#include "file_system_driver.h"
#include "syscall_helpers.h"
#include "lib.h"
#include "types.h"
#include "syscall.h"

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
    // TODO: set up stdin and stdout here
    // Boot block pointer set in kernel.c
    inode_ptr = (inode_t *)(boot_block_ptr + 1); // increase by size of pointer 
    data_block_ptr = (data_block_t *)(inode_ptr + boot_block_ptr->num_inodes);
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
    pcb_t * pcb = get_pcb_ptr();

    dentry_t file_dentry;
    int file_desc_index;
    
    // ensure the filename is valid
    if (filename == NULL || strlen(filename) > 32) {
        return -1;
    }

    // ensure the file desc has space AND find the index to emplace this file
    for (file_desc_index = 2; file_desc_index < MAX_FILE_DESC; file_desc_index++) {
        //is the index empty?
        if (pcb->file_desc_arr[file_desc_index].flags == 0) {
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
    pcb->file_desc_arr[file_desc_index].inode = file_dentry.inode_num;
    pcb->file_desc_arr[file_desc_index].flags = 1;
    pcb->file_desc_arr[file_desc_index].file_pos = 0;

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
    pcb_t * pcb = get_pcb_ptr();

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
int32_t file_read(uint32_t fd, void* buf, uint32_t nbytes) {
    pcb_t * pcb = get_pcb_ptr();

    if (pcb->file_desc_arr[fd].flags == 0) return -1;

    file_desc_t file_desc = pcb->file_desc_arr[fd];
    int32_t status = read_data(file_desc.inode, file_desc.file_pos, (uint8_t *) buf, nbytes);

    if (status == -1) return -1;
    file_desc.file_pos += status;

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
    int file_desc_index;
    dentry_t dir_dentry;
    pcb_t * pcb = get_pcb_ptr();

    // ensure the file desc has space AND find the index to emplace this file
    for (file_desc_index = 2; file_desc_index < MAX_FILE_DESC; file_desc_index++) {
        //is the index empty?
        if (pcb->file_desc_arr[file_desc_index].flags == 0) {
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

    pcb->file_desc_arr[file_desc_index].flags = 1;
    pcb->file_desc_arr[file_desc_index].inode = dir_dentry.inode_num;
    pcb->file_desc_arr[file_desc_index].file_pos = 0;

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
int32_t dir_close(int32_t fd) {
    //clear the flag and remove the file operations pointers for this process
    pcb_t * pcb = get_pcb_ptr();

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
   pcb_t * pcb = get_pcb_ptr();
    if (pcb->file_desc_arr[fd].file_pos == boot_block_ptr->num_dirs) return 0;

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
   if (pcb->file_desc_arr[fd].flags == 0) return -1;

    /* fill out the buffer based given the number of dentrys from boot_block */

    /* do 32 character name emplacement */
    dentry_t cur_file = boot_block_ptr->dir_entries[pcb->file_desc_arr[fd].file_pos];
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
    pcb->file_desc_arr[fd].file_pos++;

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

int32_t empty_open(const uint8_t * filename){
    return -1;
}

int32_t empty_close(int32_t fd){
    return -1;
}

int32_t empty_read(int32_t fd, void* buf, int32_t nbytes){
    return -1;
}

int32_t empty_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}

void init_ops_tables(){
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
}
