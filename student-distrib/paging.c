#include "paging.h"

/* page_directory is our entire page directory object, this function initializes it */
/* TODO
    INPUTS : None
    OUTPUTS : None
    RETURNS : 0 on success, -1 on failure
    SIDE EFFECTS : initializes the page directory object
*/
int init_paging() {
    int i;
    /* initialize the page directory */
    for(i = 0; i < NUM_ENTRIES; i++) {
        switch (i) {
            case 0: // Video memory page table
                page_dir[i].present = 1;
                page_dir[i].read_write = 1;
                page_dir[i].user_super = 0; //its 0 level privelage
                page_dir[i].write_through = 0;
                page_dir[i].cache_dis = 0;
                page_dir[i].accessed = 0;
                page_dir[i].reserved = 0;
                page_dir[i].page_size = 0;
                page_dir[i].global_page = 0;
                page_dir[i].available = 0;
                page_dir[i].base_31_12 = &video_memory_page_table / FOUR_KB; // TODO: check if dereference is correct
                break;
            case 1: // Kernel section (single 4mb page)
                page_dir[i].present = 1;
                page_dir[i].read_write = 1;
                page_dir[i].user_super = 0; //its 0 level privelage
                page_dir[i].write_through = 0;
                page_dir[i].cache_dis = 0;
                page_dir[i].accessed = 0;
                page_dir[i].reserved = 0;
                page_dir[i].page_size = 1;
                page_dir[i].global_page = 1;
                page_dir[i].available = 0;
                page_dir[i].base_31_12 = KERNEL_ADDRESS / FOUR_KB;
                break;
            default: // 8MB - 4GB
                page_dir[i].present = 0;
                page_dir[i].read_write = 1;
                page_dir[i].user_super = 0; //its 0 level privelage
                page_dir[i].write_through = 0;
                page_dir[i].cache_dis = 0;
                page_dir[i].accessed = 0;
                page_dir[i].reserved = 0;
                page_dir[i].page_size = 0;
                page_dir[i].global_page = 0;
                page_dir[i].available = 0;
                page_dir[i].base_31_12 = USER_ADDRESS / FOUR_KB; // TODO: is this correct?
                break;
        }
    }

    /* setup page table (the one for PDE #0)*/
    for (i = 0; i < NUM_ENTRIES; i++) {
        if(i * FOUR_KB == VIDEO){
            page_table[i].present = 1;
        }
        else{
            page_table[i].present = 0;
        }
        page_table[i].read_write = 1;
        page_table[i].user_super = 0;
        page_table[i].write_through = 0;
        page_table[i].cache_dis = 0;
        page_table[i].accessed = 0;
        page_table[i].dirty = 0;
        page_table[i].pat = 0;
        page_table[i].global_page = 0;
        page_table[i].available = 0;
        page_table[i].base_31_12 = i;
    }

    enable_paging((void *) page_dir);
}
