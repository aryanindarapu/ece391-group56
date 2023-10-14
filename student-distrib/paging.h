#include "types.h"
/* Constants that are commonly used for paging */
#define DIR_TABLE_SIZE  1024
#define FOUR_KB         4096
#define ENTRY_SIZE      4

/* steal the similar struct formats for page dir and page tables 
TODO: how do I used __attribute__((packed))/ do I even need to? */

/* Page Directory */
typedef struct __attribute__ ((packed)) page_dir{
    PDE dir[DIR_TABLE_SIZE];
} page_dir;

/* Page Directory Entry 
    - 1024 of these makes up THE page directory
*/
typedef union PDE{
    uint32_t val[1];
    struct {
        /* communicates if the particular table exists from this PDE*/
        uint32_t present : 1;
        /* SEE PAGE 50-51 of IA-32 for these variables of PDE */
        uint32_t address : 20; // [12:31]
        uint32_t avail : 3; 
        uint32_t global : 1; 
        /* page size: this one is important,
        if 1 --> 4MB pages, if 0 --> 4kB pages */
        uint32_t p_size : 1;
        uint32_t reserved : 1; //0 always
        uint32_t accessed : 1;
        uint32_t cache_dis : 1;
        uint32_t write_through : 1;
        uint32_t user_super : 1;
        uint32_t read_write : 1;
    } __attribute__ ((packed));
} PDE;

/* Page Table Entry 
    - 1024 of these makes up a page table
*/
typedef union PTE{
    uint32_t val[1];
    struct {
        /* communicates if the particular page exists from this PTE*/
        uint32_t present : 1;
        /* SEE PAGE 50-51 of IA-32 for these variables of PDE */
        uint32_t address : 20; // [12:31]
        uint32_t avail : 3; 
        uint32_t global : 1; 
        /* Page Table Attribute Index */
        uint32_t ptai : 1;
        uint32_t dirty : 1; //TODO: wtf is dirty bruh
        uint32_t accessed : 1;
        uint32_t cache_dis : 1;
        uint32_t write_through : 1;
        uint32_t user_super : 1;
        uint32_t read_write : 1;
    } __attribute__ ((packed));
} PTE;

/* our aligned page directory object and default aligned PDE/PTE objects */
page_dir page_directory __attribute__((aligned(FOUR_KB)));
PDE default_pde __attribute__((aligned(ENTRY_SIZE)));
PTE default_pte __attribute__((aligned(ENTRY_SIZE)));

/* function that instantiates out pages:
    we need: single 4MB page for the kernel (from 4MB - 8MB)
             from 8MB - 4GB should be marked not present and 4kb pages
*/
int init_paging();
