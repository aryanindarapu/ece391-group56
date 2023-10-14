

/* steal the similar struct formats for page dir and page tables */

/* Page Directory */
typedef struct __attribute__ ((packed)) page_dir{
    PDE dir[1024];
}page_dir;

/* Page Directory Entry 
    - 1024 of these makes up THE page directory
*/
typedef union PDE{
    uint32_t val[1];
    struct {
        /* communicates if the particular table exists from this PDE*/
        uint32_t present : 1;
        uint32_t rest : 31; //TODO: expand this to what it actually should represent
               /* ^ this includes address pointer to page */
        /* Potential Expansion */
        /* 
        */
    } __attribute__ ((packed));
} page_dir;

/* Page Table Entry 
    - 1024 of these makes up a page table
*/
typedef union PTE{
    uint32_t val[1];
    struct {
        /* communicates if the particular page exists from this PTE*/
        uint32_t present : 1;
        uint32_t rest : 31; //TODO: expand this to what it actually should represent
               /* ^ this includes address pointer to page */
    } __attribute__ ((packed));
} page_dir;

