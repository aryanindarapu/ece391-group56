#include "types.h"
#include "lib.h"
/* Constants that are commonly used for paging */
#define NUM_ENTRIES  1024
#define FOUR_KB      4096
#define ENTRY_SIZE   4
#define KERNEL_ADDRESS 0x400000
#define USER_ADDRESS 0x800000
#define VIDEO_ADDRESS       0xB8000

/* steal the similar struct formats for page dir and page tables 
TODO: how do I used __attribute__((packed))/ do I even need to? */

/* Page Directory Entry 
    - 1024 of these makes up THE page directory
*/
typedef union page_dir_desc_t {
    uint32_t val[1];
    struct {
        /* SEE PAGE 90-91 of IA-32 for these variables of PDE */
        uint32_t present : 1;
        /* communicates if the particular table exists from this PDE*/
        uint32_t read_write : 1;
        uint32_t user_super : 1;
        uint32_t write_through : 1;
        uint32_t cache_dis : 1;
        /* page size: this one is important,
        if 1 --> 4MB pages, if 0 --> 4kB pages */
        uint32_t accessed : 1;
        uint32_t reserved : 1; // 0 always
        uint32_t page_size : 1;
        uint32_t global_page : 1; 
        uint32_t available : 3; 
        uint32_t base_31_12 : 20; // [12:31] --> oints to a bage table
    } __attribute__ ((packed));
} page_dir_desc_t;

/* Page Table Entry 
    - 1024 of these makes up a page table
*/
typedef union page_table_desc_t {
    uint32_t val[1];
    struct {
        /* SEE PAGE 50-51 of IA-32 for these variables of PDE */
        /* communicates if the particular page exists from this PTE*/
        uint32_t present : 1;
        uint32_t read_write : 1;
        uint32_t user_super : 1;
        uint32_t write_through : 1;
        uint32_t cache_dis : 1;
        uint32_t accessed : 1;
        uint32_t dirty : 1; //TODO: wtf is dirty bruh
        /* Page Table Attribute Index */
        uint32_t pat : 1;
        uint32_t global_page : 1; 
        uint32_t available : 3; 
        uint32_t base_31_12 : 20; // [12:31]
    } __attribute__ ((packed));
} page_table_desc_t;

/* Page Directory */
page_dir_desc_t page_dir[NUM_ENTRIES] __attribute__((aligned(FOUR_KB)));
page_table_desc_t video_memory_page_table[NUM_ENTRIES] __attribute__((aligned(FOUR_KB)));
// uint32_t video_memory_page[NUM_ENTRIES] __attribute__((aligned(FOUR_KB))); //video mem page

/* 
register CR3 contains the control reigster data that points to the OS's current 
operating position
[31:22 index for the page dir, 21:12 index for the page table, 11:0 has nothing of value = 0]*/

extern int init_paging();