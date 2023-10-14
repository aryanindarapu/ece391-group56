#include "paging.h"

/* page_directory is our entire page directory object, this function initializes it */
/* TODO
    INPUTS : None
    OUTPUTS : None
    RETURNS : 0 on success, -1 on failure
    SIDE EFFECTS : initializes the page directory object
*/
int init_paging() {
    int i, j;
    // just fill out entry 0 and 1
    // go through every entry within the page directory
    unsigned int address = 0;
    for (i = 0; i < NUM_PAGING_ENTRIES; i++) {
        switch (i) {
            case 0: //video mem section
                page_dir[i].address = page_tables[i]; // should be zero since its the first page table
                page_dir[i].avail = 0; // for us, we set to zero for now
                page_dir[i].global = 1; // we dont flush the kernel page so it's global
                page_dir[i].p_size = 0; // 4 kb pages
                page_dir[i].reserved = 0;
                page_dir[i].accessed = 0; // sticky, so need to manually turn on when user accesses files and other pages
                page_dir[i].cache_dis = 0;
                page_dir[i].write_through = 0;
                page_dir[i].user_super = 0; //this is the video mem so level 0 privelage
                page_dir[i].read_write = 1; //assume read/write enabled
                page_dir[i].present = 1;
                break;
            case 1: //kernel section
                page_dir[i].address = page_tables[i]; // should be zero since its the first page table
                page_dir[i].avail = 0; // for us, we set to zero for now
                page_dir[i].global = 1; // we dont flush the kernel page so it's global
                page_dir[i].p_size = 1; // 4 mb page
                page_dir[i].reserved = 0;
                page_dir[i].accessed = 0; // sticky, so need to manually turn on when user accesses files and other pages
                page_dir[i].cache_dis = 0;
                page_dir[i].write_through = 0;
                page_dir[i].user_super = 0; //this is the video mem so level 0 privelage
                page_dir[i].read_write = 1; //assume read/write enabled
                page_dir[i].present = 1;
                break;
            default: // 8MB - 4GB 
                page_dir[i].address = page_tables[i]; // pointer to address of PDE's page table Or should it be i so it's page table array index
                page_dir[i].avail = 0; // for us, we set to zero for now
                page_dir[i].global = 0; // we dont flush the kernel page so it's global
                page_dir[i].p_size = 0; // 4 kb page
                page_dir[i].reserved = 0;
                page_dir[i].accessed = 0; // sticky, so need to manually turn on when user accesses files and other pages
                page_dir[i].cache_dis = 0;
                page_dir[i].write_through = 0;
                page_dir[i].user_super = 0; //this is the video mem so level 0 privelage(might get changed we are just doing a basic instantiation)
                page_dir[i].read_write = 0; //assume read/write enabled
                page_dir[i].present = 0;
                break;  
        }
    }

    /* Now we need to fill out the page tables */
    for (i = 0; i < NUM_PAGING_ENTRIES; i++) {
        for (j = 0; j < NUM_PAGING_ENTRIES; j++){
            switch (i) {
                case 0:
                    /* handle entries from 0mb-4mb (video mem bullshit)*/
                    break;
                case 1:
                    /* handle entries from 4mb-8mb (kernel bullshit)*/
                    break;
                default:
                    /* handle entries from 8mb-4gb (rest of the non-present paging memory) */
                    page_tables[i][j].address = page_dir[i].address + j*FOUR_KB;
                    page_tables[i][j].avail = 0;
                    page_tables[i][j].global = 0;
                    page_tables[i][j].ptai = 0;
                    page_tables[i][j].dirty = 0;
                    page_tables[i][j].accessed = 0;
                    page_tables[i][j].cahce_dis = 0;
                    page_tables[i][j].write_through = 0;
                    page_tables[i][j].user_super = 0;
                    page_tables[i][j].read_write = 0;
                    page_tables[i][j].present = 0;
                    break;
            }
        }
    } 
    
    // int i, j;
    // /* 0th entry points to the 0-4MB section that encapsulates the video memory */
    // PTE page_table_zero[DIR_TABLE_SIZE] = default_page_table; //makes a page table for this PDE
    // page_directory.dir[0].present = 1;
    // page_directory.dir[0].address = 0; //should be zero since its the first page table
    // page_directory.dir[0].avail = 0; 
    // page_directory.dir[0].global = 1; //we dont flush the kernel page so it's global
    // page_directory.dir[0].p_size = 1; //4kb pages
    // page_directory.dir[0].reserved = 0;
    // page_directory.dir[0].accessed = 0; //starts as zero since this page table is not yet accessed
    // page_directory.dir[0].cache_dis = 0;
    // page_directory.dir[0].write_through = 0;
    // page_directory.dir[0].user_super = 0; //this is the video mem so level 0 privelage
    // page_directory.dir[0].read_write = 1; //assume read/write enabled
    // /* Now I need to fill the entire page table for this first PDE */
    // for (i = 0; i < DIR_TABLE_SIZE; i++){
    //     if(i > VIDEO_MEM_BEGIN && i < VIDEO_MEM_END){
    //         page_table_zero[i].present = 1;
    //         page_table_zero[i].address = i * FOUR_KB; //TODO: THIS IS BASED ON PAGE ADDRESSING 
    //         page_table_zero[i].avail = 0;
    //         page_table_zero[i].global = 1; // we dont want to flush
    //         page_table_zero[i].ptai = 0;
    //         page_table_zero[i].dirty = 0;
    //         page_table_zero[i].accessed = 0;
    //         page_table_zero[i].cache_dis = 0;
    //         page_table_zero[i].write_through = 0;
    //         page_table_zero[i].user_super = 0;
    //         page_table_zero[i].read_write = 1;
    //     }
    //     page_table_zero[i].present = 0;
    //     page_table_zero[i].address = i * FOUR_KB; //TODO: THIS IS BASED ON PAGE ADDRESSING 
    //     page_table_zero[i].avail = 0;
    //     page_table_zero[i].global = 0;
    //     page_table_zero[i].ptai = 0;
    //     page_table_zero[i].dirty = 0;
    //     page_table_zero[i].accessed = 0;
    //     page_table_zero[i].cache_dis = 0;
    //     page_table_zero[i].write_through = 0;
    //     page_table_zero[i].user_super = 0;
    //     page_table_zero[i].read_write = 0;
    // }
    // // TODO ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    // /* 1st entry is the 4MB single kernel page 
    //     PDE for a 4MB page is structured differently
    //     [31:22]Page Base Address, [21:13]Reserved, [12]ptai, [11:9] avail,
    //     [8]global, [7]p_size, [6]dirty, [5]accessed, [4]accessed,
    //     [3]cache_dis, [2]user_super, [1]read_write, [0]present 
    // */
    // page_directory.dir[1].present = 1;
    /* the address space is split for this PDE only since its a 4MB page */
    // // /* [31:22]Page Base Address, [21:13]Reserved, [12]ptai is the new breakdown */
    // page_directory.dir[1].address = KERNEL_ADDRESS; //TODO: what should the page base address be
    // page_directory.dir[1].avail = 0; 
    // page_directory.dir[1].global = 1; //we dont flush the kernel page so it's global
    // page_directory.dir[1].p_size = 1; //4MB page
    // page_directory.dir[1].reserved = 0;
    // page_directory.dir[1].accessed = 0; //starts as zero since this page table is not yet accessed
    // page_directory.dir[1].cache_dis = 0; //TODO do we need this?
    // page_directory.dir[1].write_through = 0; //TODO do we need this?
    // page_directory.dir[1].user_super = 0; //this is the kernel page so level 0 privelage
    // page_directory.dir[1].read_write = 1; //assume read/write enabled

    // /* initial pages and page tables from 8MB - 4GB */
    // for (j = 0; j < DIR_TABLE_SIZE; j++){
    //     /* j is indexing in the page directory */
        
    // }
}