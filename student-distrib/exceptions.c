#include "exceptions.h"
#include "lib.h"

/* Handlers for exceptions in IDT in order of vector number*/

/*
 *   divide_error
 *   DESCRIPTION: handler for the divide_error exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int divide_error() {
    // cli();
    printf("Divide error occurred\n");
    while (1);
}

/*
 *   debug
 *   DESCRIPTION: handler for the debug exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int debug() {
    printf("Debug interrupt occurred\n");
    while (1);
}

/*
 *   nmi_interrupt
 *   DESCRIPTION: handler for the nmi_interrupt exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int nmi_interrupt() {
    printf("NMI interrupt occured\n");
    while (1);
}

/*
 *   breakpoint
 *   DESCRIPTION: handler for the breakpoint exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int breakpoint() {
    printf("Breakpoint interrupt occurred\n");
    while (1);
}

/*
 *   overflow
 *   DESCRIPTION: handler for the overflow exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int overflow() {
    printf("Overflow error occurred\n"); 
    while (1);
}

/*
 *   bound_range_exceeded
 *   DESCRIPTION: handler for the bound_range_exceeded exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int bound_range_exceeded() {
    printf("Bound range error occurred\n");
    while (1);
}

/*
 *   invalid_opcode
 *   DESCRIPTION: handler for the invalid_opcode exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int invalid_opcode() {
    printf("Invalid opcode error occurred\n");
    while (1);
    
}

/*
 *   device_not_available
 *   DESCRIPTION: handler for the device_not_available exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int device_not_available() {
    printf("Device not available\n");
    while (1);
}

/*
 *   double_fault
 *   DESCRIPTION: handler for the double_fault exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int double_fault() {
    printf("Double fault error occurred\n");
    while (1);
}

/*
 *   coprocessor_segment_overrun
 *   DESCRIPTION: handler for the coprocessor_segment_overrun exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int coprocessor_segment_overrun() {
    printf("Coprocessor segment overrun error occurred\n");
    while (1);
}

/*
 *   invalid_tss
 *   DESCRIPTION: handler for the invalid_tss exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int invalid_tss() {
    printf("Invalid tts error occurred\n");
    while (1);  
}

/*
 *   segment_not_present
 *   DESCRIPTION: handler for the segment_not_present exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int segment_not_present() {
    printf("***segment not present***\n");
    while (1);
}

/*
 *   stack_segment_fault
 *   DESCRIPTION: handler for the stack_segment_fault exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int stack_segment_fault() {
    printf("***stack segment fault***\n");
    while (1);
}

/*
 *   general_protection
 *   DESCRIPTION: handler for the general_protection exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int general_protection() {
    printf("General protection fault\n");
    while (1);
}

/*
 *   page_fault
 *   DESCRIPTION: handler for the page_fault exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int page_fault() {
    printf("Page fault occurred\n");
    while (1);
    // cli();
    return 256;
}

/*
 *   reserved
 *   DESCRIPTION: handler for the reserved exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int reserved() {
    printf("Reserved interrupt occurred\n");
    while (1);
}

/*
 *   x87_fpu_floating_point_error
 *   DESCRIPTION: handler for the x87_fpu_floating_point_error exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int x87_fpu_floating_point_error() {
    printf("x87 fpu floating point error occurred\n");
    while (1);
}

/*
 *   alignment_check
 *   DESCRIPTION: handler for the alignment_check exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int alignment_check() {
    printf("Alignment check interrupt occurred\n");
    while (1);
}

/*
 *   machine_check
 *   DESCRIPTION: handler for the machine_check exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int machine_check() {
    printf("Machine alignment interrupt occurred\n");
    while (1);
}

/*
 *   simd_floating_point_exception
 *   DESCRIPTION: handler for the simd_floating_point_exception exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int simd_floating_point_exception() {
    printf("Simd floating point exception occurred\n");
    while (1);
}

/*
 *   user_defined
 *   DESCRIPTION: handler for the user_defined exception, for now just prints to show interrupt has occured
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: for now just prints the current exception that has occurred
 */ 
int user_defined() {
    printf("User defined interrupt occurred\n");
    while (1);
}
