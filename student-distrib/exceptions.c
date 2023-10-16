#include "exceptions.h"

/* Handlers for exceptions in IDT in order of vector number*/

/*
 *   divide_error
 *   DESCRIPTION: handler for the divide_error exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void divide_error() {
    printf("divide error occurred\n");
    while (1);    
}

/*
 *   debug
 *   DESCRIPTION: handler for the debug exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void debug() {
    printf("debug interrupt occurred\n");
    while (1);
}

/*
 *   nmi_interrupt
 *   DESCRIPTION: handler for the nmi_interrupt exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void nmi_interrupt() {
    printf("nmi innterrupt occured\n");
    while (1);
}

/*
 *   breakpoint
 *   DESCRIPTION: handler for the breakpoint exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void breakpoint() {
    printf("breakpoint interrupt occurred\n");
    while (1);
}

/*
 *   overflow
 *   DESCRIPTION: handler for the overflow exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void overflow() {
    printf("overflow error occurred\n"); 
    while (1);
}

/*
 *   bound_range_exceeded
 *   DESCRIPTION: handler for the bound_range_exceeded exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void bound_range_exceeded() {
    printf("bound range error occurred\n");
    while (1);
}

/*
 *   invalid_opcode
 *   DESCRIPTION: handler for the invalid_opcode exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void invalid_opcode() {
    printf("invalid opcode error occurred\n");
    while (1);
    
}

/*
 *   device_not_available
 *   DESCRIPTION: handler for the device_not_available exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void device_not_available() {
    printf("device not available\n");
    while (1);
}

/*
 *   double_fault
 *   DESCRIPTION: handler for the double_fault exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void double_fault() {
    printf("double fault error occurred\n");
    while (1);
}

/*
 *   coprocessor_segment_overrun
 *   DESCRIPTION: handler for the coprocessor_segment_overrun exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void coprocessor_segment_overrun() {
    printf("coprocessor segment overrun error occurred\n");
    while (1);
}

/*
 *   invalid_tss
 *   DESCRIPTION: handler for the invalid_tss exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void invalid_tss() {
    printf("invalid tts error occurred\n");
    while (1);  
}

/*
 *   segment_not_present
 *   DESCRIPTION: handler for the segment_not_present exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void segment_not_present() {
    printf("***segment not present***\n");
    while (1);
}

/*
 *   stack_segment_fault
 *   DESCRIPTION: handler for the stack_segment_fault exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void stack_segment_fault() {
    printf("***stack segment fault***\n");
    while (1);
}

/*
 *   general_protection
 *   DESCRIPTION: handler for the general_protection exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void general_protection() {
    printf("general protection fault\n");
    while (1);
}

/*
 *   page_fault
 *   DESCRIPTION: handler for the page_fault exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void page_fault() {
    printf("page fault occurred\n");
    while (1);
}

/*
 *   reserved
 *   DESCRIPTION: handler for the reserved exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void reserved() {
    printf("reserved interrupt occurred\n");
    while (1);
}

/*
 *   x87_fpu_floating_point_error
 *   DESCRIPTION: handler for the x87_fpu_floating_point_error exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void x87_fpu_floating_point_error() {
    printf("x87 fpu floating point error occurred\n");
    while (1);
}

/*
 *   alignment_check
 *   DESCRIPTION: handler for the alignment_check exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void alignment_check() {
    printf("alignment check interrupt occurred\n");
    while (1);
}

/*
 *   machine_check
 *   DESCRIPTION: handler for the machine_check exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void machine_check() {
    printf("machine alignment interrupt occurred\n");
    while (1);
}

/*
 *   simd_floating_point_exception
 *   DESCRIPTION: handler for the simd_floating_point_exception exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void simd_floating_point_exception() {
    printf("simd floating point exception occurred\n");
    while (1);
}

/*
 *   user_defined
 *   DESCRIPTION: handler for the user_defined exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void user_defined() {
    printf("user defined interrupt occurred\n");
    while (1);
}

/*
 *   system_call
 *   DESCRIPTION: handler for the system_call exception, for now just prints to show Interupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
void system_call() {
    printf("system call occurred\n");
    while (1);   
}
