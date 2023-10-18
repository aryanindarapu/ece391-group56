.global system_handler

jump_table:
    .long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

system_handler:
    # Check the value of EAX to make sure it is valid
    # EAX can only be between 1 and 10
    cmpl $11, %eax
    jge INVALID

    # EAX is valid range so now we call jump table
    jmp *jump_table(, %eax, 4)

    # Bound was invalid so we return -1
    INVALID:
        movl $-1, %eax
        ret

halt:
execute:
read:
# EBX: fd
# ECX: buf
# EDX: nbytes
    pushl %ebp
	movl %esp, %ebp


write:
# EBX: fd
# ECX: const buf
# EDX: nbytes
    pushl %ebp
	movl %esp, %ebp

open:
# EBX: * filename
    pushl %ebp
	movl %esp, %ebp


close:
# EBX: fd
    pushl %ebp
	movl %esp, %ebp

# currently non-implemented jump_table symbols 
getargs:
vidmap:
set_hndler:
sigreturn:


