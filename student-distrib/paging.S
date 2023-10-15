# TODO: make a paging initalization function
# TODO: how do we allocate an entire 4kB memory set for one page


# TODO: set cr3 reg here and flush tlbs
# TODO: cr2 is page fault linear address --> if not null, then page fault
#define ASM 1

# one argument is page directory
.globl enable_paging
.align 4
enable_paging:
    # 1. callee setup
    movl %esp, %ebp
    pushl %ebp
    # callee save 
    pushl %ebx
    pushl %esi
    pushl %edi
    
    # 2. set cr3 to point to page directory
    movl 8(%ebp), %eax 
    movl %eax, %cr3

    # 3. enable paging bit (bit 31 and bit 0) in cr0
    movl %cr0, %eax
    orl $0x80000001, %eax
    movl %eax, %cr0

    # 4. enable page size extension (bit 4) for 4MB page size in cr4
    mov %eax, %cr4
    or %eax, 0x00000010
    mov %cr4, %eax

    # callee_teardown
    popl %edi
    popl %esi
    popl %ebx

    # flush TLB by reloading the CR3 register
    movl %cr3, %eax
    movl %eax, %cr3

    leave
    ret