.global system_handler
.global end_system_call
.global dir_op
.global rtc_op
.global file_op

# note that the first jump table entry is 0x0 since 0 isn't a system call entry number
jump_table:
    .long 0x0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

system_handler:
    # COMMENTED OUT BECAUSE I DON'T KNOW IF WE NEED THIS SETUP
    # callee setup
    # pushl %ebp
    # movl %esp, %ebp

    # callee save
    # pushl %esi 
    # pushl %edi
    # pushl %eax 
    # pushl %ebx 
    # pushl %ecx 
    # pushl %edx

    # send arguments in ebx ecx and edx onto stack for the jump symbols function
    pushl %edx # third arg
    pushl %ecx # second arg
    pushl %ebx # first arg

    # Check the value of EAX to make sure it is valid
    # ensure the opcode is at least one
    cmpl $0, %eax
    jle INVALID
    # EAX can only be between 1 and 10
    cmpl $11, %eax
    jge INVALID

    # EAX is valid range so now we call jump table
    # Jump Table symbols are in C so we can program more easily
    jmp *jump_table(, %eax, 4)

    # Bound was invalid so we return -1
    INVALID:
        movl $-1, %eax 
        # release args from the stack
        popl %ebx
        popl %ecx
        popl %edx
        ret

# not sure if this is a valid symbol or if it's C convention to pop args off the stack after the 
# function call
end_system_call:
    # return success
    movl $0, %eax 
    # release args from the stack
    popl %ebx
    popl %ecx
    popl %edx
    ret
# COMMENTED OUT SINCE WE SHOULDN'T PROGRAM SYMBOLS IN x86, EASIER TO IMPLEMENT IN C
# halt:
# execute:
# read:
# EBX: fd
# ECX: buf
# EDX: nbytes
#    pushl %ebp
#  	movl %esp, %ebp


# write:
# EBX: fd
# ECX: const buf
# EDX: nbytes
#     pushl %ebp
# 	movl %esp, %ebp

# open:
# EBX: * filename
#    pushl %ebp
# 	movl %esp, %ebp


# close:
# EBX: fd
#    pushl %ebp
# 	movl %esp, %ebp

# currently non-implemented jump_table symbols 
# getargs:
# vidmap:
# set_handler:
# sigreturn:

# jumptable for directory operations
dir_ops_table:
.long dir_open, dir_close, dir_read, dir_write

# jumptable for rtc operations
rtc_ops_table:
.long rtc_open, rtc_close, rtc_read, rtc_write

# jumptable for regular file operations
file_ops_table:
.long file_open, file_close, file_read, file_write


# argument is opcode : 
# 0-->open
# 1-->close
# 2-->read
# 3-->write

# C callable function symbol for directory operations
dir_op:
    # callee setup
    pushl %ebp
    movl %esp, %ebp

    # callee save
    pushl %esi 
    pushl %edi
    pushl %eax 
    pushl %ebx 
    pushl %ecx 
    pushl %edx

    # check opcode validity
    movl 8(&ebp), %ebx
    cmpl $0, %ebx
    jl op_invalid
    cmpl $3, %ebx
    jg op_invalid

    # use the jumptable to get to the correct opcode function
    jmp *dir_ops_table(, %ebx, 4)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# C callable function symbol for rtc operations
rtc_op:
    # callee setup
    pushl %ebp
    movl %esp, %ebp

    # callee save
    pushl %esi 
    pushl %edi
    pushl %eax 
    pushl %ebx 
    pushl %ecx 
    pushl %edx

    # check opcode validity
    movl 8(&ebp), %ebx
    cmpl $0, %ebx
    jl op_invalid
    cmpl $3, %ebx
    jg op_invalid

    # use the jumptable to get to the correct opcode function
    jmp *rtc_ops_table(, %ebx, 4)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# C callable function symbol for regular file operations
file_op:
# callee setup
    pushl %ebp
    movl %esp, %ebp

    # callee save
    pushl %esi 
    pushl %edi
    pushl %eax 
    pushl %ebx 
    pushl %ecx 
    pushl %edx

    # check opcode validity
    movl 8(&ebp), %ebx
    cmpl $0, %ebx
    jl op_invalid
    cmpl $3, %ebx
    jg op_invalid

    # use the jumptable to get to the correct opcode function
    jmp *file_ops_table(, %ebx, 4)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

op_invalid: 
    # callee teardown and dip
    popl %edx
    popl %ecx 
    popl %ebx
    popl %eax
    popl %edi
    popl %esi
    ret

