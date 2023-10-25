#include "types.h"
#include "file_system_driver.h"
#include "syscall.h"

/* file system helper functions */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

pcb_t * get_pcb_ptr (void);
