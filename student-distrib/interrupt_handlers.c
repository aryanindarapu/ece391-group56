#include "interrupt_handlers.h"

void divide_error() {
    // TODO: do a while 1 loop
    printf("divide error occurred\n");
    while (1);
    
    return 0;
}

void nmi_interrupt() {
    printf("nmi innterrupt occured\n");

    while (1);

    return 0;
};

void breakpoint() {
    printf("breakpoint interrupt occurred\n");

    while (1);

    return 0;
};

void overflow() {
    printf("overflow error occurred\n");
    
    while (1);

    return 0;
};

void bound_range_exceeded() {
    printf("bound range error occurred\n");

    while (1);

    return 0;
};

void invalid_opcode() {
    printf("invalid opcode error occurred\n");

    while (1);

    return 0;
};

void device_not_available(){
    printf("device not available\n");

    while (1);

    return 0;
};

void double_fault(){
    printf("double fault error occurred\n");

    while (1);

    return 0;
};

void coprocessor_segment_overrun(){
    printf("coprocessor segment overrun error occurred\n");

    while (1);

    return 0;
};

void invalid_tss(){
    printf("invalid tts error occurred\n");

    while (1);

    return 0;
};

void segment_not_present(){
    printf("***segment not present***\n");

    while (1);

    return 0;
};

void stack_segment_fault(){
    printf("***stack segment fault***\n");

    while (1);

    return 0;
};

void general_protection(){
    printf("general protection fault\n");
    
    while (1);

    return 0;
};

void page_fault(){
    printf("page fault occurred\n");

    while (1);

    return 0;
};

void x87_fpu_floating_point_error(){
    printf("x87 fpu floating point error occurred\n");

    while (1);

    return 0;
}

void alignment_check(){
    printf("alignment check interrupt occurred\n");

    while (1);

    return 0;
}
void machine_alignment(){
    printf("machine alignment interrupt occurred\n");

    while (1);

    return 0;
}
void simd_floating_point_exception(){
    printf("simd floating point exception occurred\n");

    while (1);

    return 0;
}
void user_defined(){
    printf("user defined interrupt occurred\n");

    while (1);

    return 0;
}

void system_call() {
    printf("system call occurred\n");

    while (1);

    return 0;
}
