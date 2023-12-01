#include "exceptions.h"
#include "lib.h"
#include "syscall.h"

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
    // printf("Divide error occurred\n");
    exception_raised_flag = 0;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Debug interrupt occurred\n");
    exception_raised_flag = 1;
    halt((uint8_t) 256);
    return 256;
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
    // printf("NMI interrupt occured\n");
    exception_raised_flag = 1;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Breakpoint interrupt occurred\n");
    exception_raised_flag = 2;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Overflow error occurred\n"); 
    exception_raised_flag = 3;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Bound range error occurred\n");
    exception_raised_flag = 4;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Invalid opcode error occurred\n");
    exception_raised_flag = 5;
    halt((uint8_t) 256);
    
    return 256;
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
    // printf("Device not available\n");
    exception_raised_flag = 6;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Double fault error occurred\n");
    exception_raised_flag = 7;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Coprocessor segment overrun error occurred\n");
    exception_raised_flag = 8;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Invalid tts error occurred\n");
    exception_raised_flag = 9;
    halt((uint8_t) 256);  
    return 256;
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
    // printf("***segment not present***\n");
    exception_raised_flag = 10;
    halt((uint8_t) 256);
    return 256;
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
    // printf("***stack segment fault***\n");
    exception_raised_flag = 11;
    halt((uint8_t) 256);
    return 256;
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
    // printf("General protection fault\n");
    exception_raised_flag = 12;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Page fault occurred\n");
    exception_raised_flag = 13;
    halt((uint8_t) 256);
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
    // printf("Reserved interrupt occurred\n");
    exception_raised_flag = 14;
    halt((uint8_t) 256);
    return 256;
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
    // printf("x87 fpu floating point error occurred\n");
    exception_raised_flag = 15;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Alignment check interrupt occurred\n");
    exception_raised_flag = 16;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Machine alignment interrupt occurred\n");
    exception_raised_flag = 17;
    halt((uint8_t) 256);
    return 256;
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
    // printf("Simd floating point exception occurred\n");
    exception_raised_flag = 18;
    halt((uint8_t) 256);
    return 256;
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
    // printf("User defined interrupt occurred\n");
    exception_raised_flag = 19;
    halt((uint8_t) 256);
    return 256;
}
