#include "paging.h"




/* page_directory is our entire page directory object, this function initializes it */
/* TODO
    INPUTS : None
    OUTPUTS : None
    RETURNS : 0 on success, -1 on failure
    SIDE EFFECTS : initializes the page directory object
*/
int init_paging () {
    int i;
 
    /* initialize the page directory */
    for(i = 0; i < NUM_ENTRIES; i++) {
        switch (i) {
            case 0: // Video memory page table
                page_dir[i].p = 1;
                page_dir[i].rw = 1;
                page_dir[i].us = 0; //its 0 level privelage
                page_dir[i].pwt = 0;
                page_dir[i].pcd = 0;
                page_dir[i].a = 0;
                page_dir[i].res = 0;
                page_dir[i].ps = 0;
                page_dir[i].g = 0;
                page_dir[i].avail = 0;
                page_dir[i].table_base_addr = (unsigned int) (&video_memory_page_table) / FOUR_KB; // TODO: check if dereference is correct
                break;
            case 1: // Kernel section (single 4mb page)
                page_dir[i].p = 1;
                page_dir[i].rw = 1;
                page_dir[i].us = 0; //its 0 level privelage
                page_dir[i].pwt = 0;
                page_dir[i].pcd = 1;
                page_dir[i].a = 0;
                page_dir[i].res = 0;
                page_dir[i].ps = 1;
                page_dir[i].g = 1;
                page_dir[i].avail = 0;
                page_dir[i].table_base_addr = KERNEL_ADDRESS / FOUR_KB;
                break;
            default: // 8MB - 4GB
                page_dir[i].p = 0;
                page_dir[i].rw = 1;
                page_dir[i].us = 0; //its 0 level privelage
                page_dir[i].pwt = 0;
                page_dir[i].pcd = 0;
                page_dir[i].a = 0;
                page_dir[i].res = 0;
                page_dir[i].ps = 0;
                page_dir[i].g = 0;
                page_dir[i].avail = 0;
                page_dir[i].table_base_addr = 0; //USER_ADDRESS / FOUR_KB; // TODO: is this correct?
                break;
        }
    }

    /* setup page table (the one for PDE #0)*/
    for (i = 0; i < NUM_ENTRIES; i++) {
        if(i * FOUR_KB == VIDEO_ADDRESS){
            video_memory_page_table[i].p = 1;
        }
        else{
            video_memory_page_table[i].p = 0;
        }
        video_memory_page_table[i].rw = 1;
        video_memory_page_table[i].us = 0;
        video_memory_page_table[i].pwt = 0;
        video_memory_page_table[i].pcd = 0;
        video_memory_page_table[i].a = 0;
        video_memory_page_table[i].d = 0;
        video_memory_page_table[i].pat = 0;
        video_memory_page_table[i].g = 0;
        video_memory_page_table[i].avail = 0;
        video_memory_page_table[i].page_base_addr = i;
    }
    
    loadPageDirectory((unsigned int *) (&page_dir));
    enablePaging();

    return 0;
}
