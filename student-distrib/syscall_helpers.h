#include "types.h"
#include "file_system_driver.h"
#include "syscall.h"
#include "process.h"

#define PCB_BITMASK 0xFFFFE000

/* file system helper functions */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

pcb_t * get_curr_pcb_ptr (void);
pcb_t * get_pcb_ptr(int32_t pid);

void setup_user_page(uint32_t table_addr);
