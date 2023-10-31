.global load_page_dir
.global enable_paging
.global flush_tlb


# load_page_dir
#   DESCRIPTION: Loads the page directory into the cr3 register
#   INPUTS: unsigned int * page_directory - pointer to the page directory
#   OUTPUTS: none
#   RETURN VALUE: none
#   SIDE EFFECTS: sets the cr3 register to the page directory
load_page_dir:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%esp), %eax
    movl %eax, %cr3 # NOTE: this flushes the TLBs
    leave
    ret


# enable_paging
#   DESCRIPTION: Enables paging within the control registers
#   INPUTS: none
#   OUTPUTS: none
#   RETURN VALUE: none
#   SIDE EFFECTS: sets the cr0 and cr4 registers to enable paging
enable_paging:
    pushl %ebp
    movl %esp, %ebp

    # Set bit 4 in CR4 to enable 4MB pages
    mov %cr4, %eax
    or $0x00000010, %eax
    mov %eax, %cr4

    # Need to set 32nd and 1st bit in CR0 to enable paging
    mov %cr0, %eax
    or $0x80000001, %eax
    mov %eax, %cr0
    leave
    ret

# flush_tlb
#   DESCRIPTION: Flushes the TLBs
#   INPUTS: none
#   OUTPUTS: none
#   RETURN VALUE: none
#   SIDE EFFECTS: flushes the TLBs
flush_tlb:
    movl %cr3, %eax
    movl %eax, %cr3
    ret
    