.global system_call_handler
.global end_halt

# note that the first jump table entry is 0x0 since 0 isn't a system call entry number
sys_call_table:
    .long 0x0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

system_call_handler:
    
    # Check the value of EAX to make sure it is valid
    # ensure the opcode is at least one
    cmpl $0, %eax
    jle INVALID

    # EAX can only be between 1 and 10
    cmpl $11, %eax
    jge INVALID
    
    # save regs other than return reg since we do call the jump table that points to a C symbol
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi 
    pushl %edi
    pushl %ebp
    # pushl %eax
    # pushfl

    # send arguments in ebx ecx and edx onto stack for the jump symbols function
    pushl %edx # third arg
    pushl %ecx # second arg
    pushl %ebx # first arg

    # EAX is valid range so now we call jump table
    # Jump Table symbols are in C so we can program more easily
    call *sys_call_table(, %eax, 4)
    jmp end_sys_call


end_sys_call:
    # release args from the stack
    addl $12, %esp
    
    # callee teardown (from jumptable C function)
    # popfl
    # popl %eax
    popl %ebp
    popl %edi
    popl %esi
    popl %edx
    popl %ecx
    popl %ebx
    
    # return from interrupt handler
    iret

# Bound was invalid so we return -1
INVALID:
    movl $-1, %eax 
    
    iret

# helper functions for halt, probably could be achieved in asm volatile
end_halt:
    pushl %ebp
    movl %esp, %ebp

    # clear regs and get arguments
    xorl %ebx, %ebx
    xorl %ecx, %ecx
    xorl %edx, %edx
    movl 8(%ebp), %ebx      # grab ebp
    movl 12(%ebp), %ecx    # grab esp
    movl 16(%ebp), %edx     # grab status
    
    # emplace args to proper register to restore execute function context
    movl %ebx, %ebp
    movl %ecx, %esp   
    movl %edx, %eax # return status

    leave
    # notice this isn't iret since we are just return back to execute
    ret

# recover_context_from_halt:



