# TODO: make a paging initalization function
# TODO: how do we allocate an entire 4kB memory set for one page


# TODO: set cr3 reg here and flush tlbs
# TODO: cr2 is page fault linear address --> if not null, then page fault
# #define ASM 1

# Loading the page directory
.global loadPageDirectory
.global enablePaging

loadPageDirectory:
pushl %ebp
movl %esp, %ebp
movl 8(%esp), %eax
# Put address of page directory into the CR3 register
# Need to do assembly since C code cannot access CR3
shll $12, %eax 
movl %eax, %cr3
leave
ret

# Enable paging
enablePaging:
pushl %ebp
movl %esp, %ebp

# Set bit 4 in CR4 to enable $MB pages
mov %cr4, %eax
or $0x8, %eax
mov %eax, %cr4

# Need to set 32nd bit in CR0 to enable paging
mov %cr0, %eax
or $0x80000000, %eax
mov %eax, %cr0
leave
ret
