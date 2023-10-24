.global system_call_handler
# .global dir_ops_table
# .global rtc_ops_table
# .global file_ops_table

# note that the first jump table entry is 0x0 since 0 isn't a system call entry number
jump_table:
    .long 0x0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

system_call_handler:
    # COMMENTED OUT BECAUSE I DON'T KNOW IF WE NEED THIS SETUP
    # callee setup
    # pushl %ebp
    # movl %esp, %ebp

    # # callee save
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

# # jumptable for directory operations
# dir_ops_table:
# .long dir_open, dir_close, dir_read, dir_write

# # jumptable for rtc operations
# rtc_ops_table:
# .long rtc_open, rtc_close, rtc_read, rtc_write

# # jumptable for regular file operations
# file_ops_table:
# .long file_open, file_close, file_read, file_write

# argument is opcode : 
# 0-->open
# 1-->close
# 2-->read
# 3-->write
