// Add other stuff we need to include
#include "rtc.h"

int rtc_interrupt_flag;

void *get_physaddr(void *virtualaddr);

void map_page(void *physaddr, void *virtualaddr, unsigned int flags);

uint32_t read_dentry_by_name (const uint8 t* fname, dentry t* dentry);

uint32_t read_dentry_by_index (uint32 t index, dentry t* dentry);

uint32_t read_data (uint32 t inode, uint32 t offset, uint8 t* buf, uint32 t length);

typedef struct inode_t{
    int32_t length;
    int32_t data_block_num[1023];
} inode_t;

typedef struct dentry_t{
    int8_t filename[FILENAME_LEN];
    int32_t filetype;
    int32_t inode_num;
    int8_t reserved[24];
} dentry_t;

typedef struct boot_block_t{
    int32_t dir_count;
    int32_t inode_count;
    int32_t data_count;
    int8_t reserved[52];
    dentry_t direntries[63];
} boot_block_t;