#include "file_system.h"


// file_sys_t file_system; // TODO: do i make this static?

// TODO: comment
// Initialize the file system
void init_file_system(void) {
    // TODO: idk what mem address to use
    // boot_block_ptr = (boot_block_t *)mem_addr
    uint32_t num_inodes = boot_block_ptr->num_inodes;
    inode_ptr = (inode_t *)(boot_block_ptr + 1); 
    data_block_ptr = (uint8_t *)(inode_ptr + num_inodes);
}

/* https://wiki.osdev.org/Paging */
/* */
/* BOTH OF THESE FUNCTIONS ARE DIRECTLY RIPPED FROM OS DEV PAGING */

/* gets the physical address given the virtual address 
    INPUTS : virtualaddr which is the value of CR3 register holding the virualized address 
*/
void *get_physaddr (void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
 
    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}


void map_page (void *physaddr, void *virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
 
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
}

// THIS FUNCTION WILL BE CALLED IN OUR SYS_OPEN() SYSTEM CALL
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry) {
    int dir_index;

    // Scan through directory entries to find file name
    for (dir_index = 0; dir_index < boot_block_ptr->num_dirs; dir_index++) {
        if (boot_block_ptr->dir_entries[dir_index].file_name == fname) {
            // File found in our boot block so we update our dentry
            strcpy(dentry->file_name, fname);
            // Now we update the dentry with the inode and type
            return read_dentry_by_index(dir_index, dentry);
        }
    }
    
    // File not found
    return -1;

}

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry) {
    // Open up a file and set up the file object
    // NOTE: index is the directory index
    if (index >= boot_block_ptr->num_inodes) {
        return -1;
    }
    
    dentry_t boot_block_dentry_list = boot_block_ptr->dir_entries[index];
    
    dentry->file_type = boot_block_dentry_list.file_type;
    dentry->inode_num = boot_block_dentry_list.file_name;

    return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {

}
