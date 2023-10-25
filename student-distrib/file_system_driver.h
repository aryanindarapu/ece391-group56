#ifndef _FILE_SYSTEM_DRIVER_H
#define _FILE_SYSTEM_DRIVER_H

#include "devices/rtc.h"
#include "terminal.h"


#define FILENAME_SIZE 32
#define DATA_BLOCK_SIZE 4096
#define MAX_FILE_DESC 8 
#define DATA_BLOCKS_PER_INODE 1023
#define FORMATTER_LENGTH 11

int rtc_interrupt_flag;

typedef struct template_ops_table {
    int32_t (*open) (const uint8_t* filename);
    int32_t (*close) (uint32_t fd);
    int32_t (*read) (uint32_t fd, void* buf, uint32_t nbytes);
    int32_t (*write) (uint32_t fd, const void* buf, uint32_t nbytes);
} template_ops_table_t;

// within boot block
typedef struct dentry_t {
    char file_name[FILENAME_SIZE]; // 32B for the file name so 32 8-bit values
    int32_t file_type; // Will be 0 for RTC, 1 for dir., and 2 for file
    int32_t inode_num; // Which inode holds the data block information
    uint8_t reserved[24]; // 24 - number of reserved bytes in dentry
} dentry_t;

// size - 4kB
typedef struct boot_block_t {
    int32_t num_dirs; // # of entries in our directory
    int32_t num_inodes; // # of inodes
    int32_t num_data_blocks; // # of total data blocks
    uint8_t reserved[52]; // reserved 52 bytes
    dentry_t dir_entries[63]; // list of dir. entries (63 so total size of boot is 4kB)
} boot_block_t;

// size - 4kB
typedef struct inode_t {
    int32_t length; // length in bytes of data blocks
    uint32_t data_blocks[DATA_BLOCKS_PER_INODE]; // INDEX of the data block (data is NOT continuous per block)
} inode_t;

// size - 4kB
typedef struct data_block_t {
    uint8_t data[DATA_BLOCK_SIZE]; // 4kB of data
} data_block_t;

// file descriptor struct
typedef struct file_desc_t {
    template_ops_table_t ops_ptr; // TODO: do we need a pointer
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flags;
} file_desc_t;

//THIS CODE IS CAUSING SOME PROBLEMS I THINK
//TODO: ary help me debug the errors i'm getting on the current build, I really don't know whats going on and its
// not even an assembly issue
// TODO: ask TA if this is correct

/* file system initialization */
void init_file_system(void);

/* file system operations */
int32_t file_open(const uint8_t * filename);
int32_t file_close(uint32_t fd);
int32_t file_read(uint32_t fd, void* buf, uint32_t nbytes);
int32_t file_write(uint32_t fd, const void* buf, uint32_t nbytes);


/* directory syscall functions */
int32_t dir_open(const uint8_t * filename);
int32_t dir_close(uint32_t fd);
int32_t dir_read(uint32_t fd, void* buf, uint32_t nbytes);
int32_t dir_write(uint32_t fd, const void* buf, uint32_t nbytes);

template_ops_table_t dir_ops_table = {
    dir_open,
    dir_close,
    dir_read,
    dir_write
};

template_ops_table_t terminal_ops_table = {
    NULL,
    NULL,
    NULL,
    terminal_write
};

template_ops_table_t file_ops_table = {
    file_open,
    file_close,
    file_read,
    file_write
};

// // TODO: is this correct? -- do we need fake function that returns -1 instead of NULL?
// template_ops_table_t keyboard_ops_table = {
//     NULL,
//     NULL,
//     terminal_read,
//     NULL
// };

/* file system instantiation */
boot_block_t * boot_block_ptr; // Pointer to our boot block
inode_t * inode_ptr; // List of inodes
data_block_t * data_block_ptr; // Pointer to our data blocks

/* file descriptor array */
// file_desc_t file_desc_arr[MAX_FILE_DESC];


#endif /* _FILE_SYSTEM_DRIVER_H */
