// Add other stuff we need to include
#include "rtc.h"

#define FILENAME_SIZE 32

int rtc_interrupt_flag;

void *get_physaddr(void *virtualaddr);

void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

uint32_t read_dentry_by_name (const uint8 t* fname, dentry t* dentry);

uint32_t read_dentry_by_index (uint32 t index, dentry t* dentry);

uint32_t read_data (uint32 t inode, uint32 t offset, uint8 t* buf, uint32 t length);

typedef struct file_sys_t {
    boot_block_t * boot_block; // Pointer to our boot block
    inode_t * inodes; // List of inodes
    // MP3.2 TODO: Check how in the absolute fuck we are creating pointer to data blocks??
    int8_t * data_blocks; // Pointer to our data blocks
} file_sys_t;

typedef struct inode_t{
    int32_t length; // How many data block entries there are
    int32_t data_block_num[1023]; // INDEX of the data block (data is NOT continuous per block)
} inode_t;

typedef struct dentry_t{
    int8_t filename[FILENAME_SIZE]; // 32B for the file name so 32 8-bit values
    int32_t filetype; // Will be 0 for RTC, 1 for dir., and 2 for file
    int32_t inode_num; // Which inode holds the data block information
    int8_t reserved[24]; // reserved
} dentry_t;

typedef struct boot_block_t{
    int32_t dir_count; // # of entries in our directory
    int32_t inode_count; // # of inodes
    int32_t data_count; // # of total data blocks
    int8_t reserved[52]; // reserved
    dentry_t direntries[63]; // list of dir. entries (63 so total size of boot is 4kB)
} boot_block_t;