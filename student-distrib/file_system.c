#include "file_system.h"


file_sys_t file_system; // TODO: do i make this static?


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
    for (dir_index = 0; dir_index < file_system.boot_block.dir_count; dir_index++) {
        if (file_system.boot_block.dir_entries[dir_index].file_name == fname) {
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
    if (index >= file_system.boot_block.inode_count) {
        return -1;
    }
    
    dentry_t boot_block_dentry = file_system.boot_block.dir_entries[index];
    dentry->file_type = boot_block_dentry.file_type;
    dentry->inode_num = boot_block_dentry.file_name;

    return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {

}
