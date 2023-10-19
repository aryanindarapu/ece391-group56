// Add other stuff we need to include
#include "rtc.h"
#include "types.h"

#define FILENAME_SIZE 32
#define DATA_BLOCK_SIZE 4096
#define MAX_FILE_DESC 8 

int rtc_interrupt_flag;

typedef struct file_sys_t {
    boot_block_t boot_block; // Pointer to our boot block
    inode_node_t * inodes; // List of inodes
    // MP3.2 TODO: Check how in the absolute fuck we are creating pointer to data blocks??
    data_block_t * data_blocks; // Pointer to our data blocks
} file_sys_t;

// size - 4kB
typedef struct boot_block_t {
    int32_t dir_count; // # of entries in our directory
    int32_t inode_count; // # of inodes
    int32_t data_count; // # of total data blocks
    uint8_t reserved[52]; // reserved 52 bytes
    dentry_t dir_entries[63]; // list of dir. entries (63 so total size of boot is 4kB)
} boot_block_t;

// size - 4kB
// TODO: figure out if we can include this next pointer here
typedef struct inode_t {
    int32_t length; // How many data block entries there are
    data_block_t data_block_num[1023]; // INDEX of the data block (data is NOT continuous per block)
    inode_t * next;
} inode_t;

// typedef struct inode_node_t {
//     inode_t * inode;
//     inode_node_t * next;
// } inode_node_t;

typedef struct data_block_t {
    uint8_t data_block[DATA_BLOCK_SIZE];
    data_block_t * next;
} data_block_t;

// typedef struct data_block_node_t {
//     data_block_t * data_block;
//     data_block_node_t * next;
// } data_block_node_t;


// within boot block
typedef struct dentry_t {
    char file_name[FILENAME_SIZE]; // 32B for the file name so 32 8-bit values
    int32_t file_type; // Will be 0 for RTC, 1 for dir., and 2 for file
    int32_t inode_num; // Which inode holds the data block information
    uint8_t reserved[24]; // reserved
} dentry_t;

// for file descriptor array ONLY
typedef struct file_desc_t {
    uint32_t file_ops_ptr;
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flags;
} file_desc_t;

void* get_physaddr(void *virtualaddr);
void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

void init_file_system(void);

/* file system instantiation */
file_sys_t file_sys;

//no dentries, no inodes, no data blocks, and no pointers to dentries


static file_desc_t file_desc_arr[MAX_FILE_DESC];
static uint8_t file_desc_index = 0;
