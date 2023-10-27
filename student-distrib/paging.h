#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"
#include "lib.h"

/* Constants that are commonly used for paging */
#define NUM_ENTRIES  1024
#define FOUR_KB      4096
#define ENTRY_SIZE   4
#define KERNEL_ADDRESS 0x400000
#define VIDEO_ADDRESS  0xB8000

#ifndef ASM

/* Initialize paging */
void init_paging();

// Load page directory with our page directory point
/*
   INPUTS: Pointer to the page directory (which is really a pointer to the first PDE)
   OUTPUT: NONE
   RETURN VALUE: NONE
   SIDE EFFECTS: Flushes TLB, and load the page directory base address
*/
extern void load_page_dir(unsigned int *);

// Enable paging for general purpose
/*
   INPUTS: NONE
   OUTPUT: NONE
   RETURN VALUE: NONE
   SIDE EFFECTS: sets the cr0 and cr4 registers to enable paging
*/
extern void enable_paging();

// Flushes the TLB
extern void flush_tlb();


/* Page directory descriptor */
typedef union page_dir_desc_t {
    uint32_t val[1];
    struct {
        /* SEE PAGE 90-91 of IA-32 for these variables of PDE */
        uint32_t p : 1; // 1 if the page is present, 0 if not present
        uint32_t rw : 1; // See if we are allowed to read and write
        uint32_t us : 1; // 0 is supervisor privelege, 1 is user privileege
        uint32_t pwt : 1; // write-through caching enabling
        uint32_t pcd : 1; // cache disabled
        /* page size: this one is important,
        if 1 --> 4MB pages, if 0 --> 4kB pages */
        uint32_t a : 1; // read or has been written to
        uint32_t d : 1; // dirty
        uint32_t ps : 1; // size of page whetehr it is 4KB or 4MB
        uint32_t g : 1; // ind  icates a page is global
        uint32_t avail : 3; // free 3 bits
        uint32_t table_base_addr : 20; // [12:31] --> points to a page table
    } __attribute__ ((packed));
} page_dir_desc_t;

/* Page table descriptor */
typedef union page_table_desc_t {
    uint32_t val[1];
    struct {
        /* SEE PAGE 50-51 of IA-32 for these variables of PDE */
        /* communicates if the particular page exists from this PTE*/
        uint32_t p : 1; // 1 if the page is present, 0 if not present
        uint32_t rw : 1; // See if we are allowed to read and write
        uint32_t us : 1; // 0 is supervisor privelege, 1 is user privileege
        uint32_t pwt : 1; // write-through caching enabling
        uint32_t pcd : 1; // cache disabled
        uint32_t a : 1; // read or has been written to
        uint32_t d : 1; // Changed when you are holding onto the page, first time when accessed for write
        uint32_t pat : 1; // Page attribute table
        uint32_t g : 1; // Indicates page is global
        uint32_t avail : 3; // 3 available bits
        uint32_t page_base_addr : 20; // [12:31]
    } __attribute__ ((packed));
} page_table_desc_t;

page_dir_desc_t page_dir[NUM_ENTRIES] __attribute__((aligned(FOUR_KB)));
page_table_desc_t video_memory_page_table[NUM_ENTRIES] __attribute__((aligned(FOUR_KB)));

#endif /* ASM */

#endif /* _PAGING_H */
