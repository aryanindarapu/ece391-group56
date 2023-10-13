#include "interrupt_handlers.h"

int divide_error() {
    // TODO: do a while 1 loop
    printf("divide error occurred\n");

    while (1);
    
    return 0;
}

int nmi_interrupt() {
    printf("nmi innterrupt occured\n");

    while (1);

    return 0;
};

int breakpoint() {
    printf("breakpoint interrupt occurred\n");

    while (1);

    return 0;
};

int overflow() {
    printf("overflow error occurred\n");
    
    while (1);

    return 0;
};

int bound_range_exceeded() {
    printf("bound range error occurred\n");

    while (1);

    return 0;
};

int invalid_opcode() {
    printf("invalid opcode error occurred\n");

    while (1);

    return 0;
};

int device_not_available(){
    printf("device not available\n");

    while (1);

    return 0;
};

int double_fault(){
    printf("double fault error occurred\n");

    while (1);

    return 0;
};

int coprocessor_segment_overrun(){
    printf("coprocessor segment overrun error occurred\n");

    while (1);

    return 0;
};

int invalid_tss(){
    printf("invalid tts error occurred\n");

    while (1);

    return 0;
};

int segment_not_present(){
    printf("***segment not present***\n");

    while (1);

    return 0;
};

int stack_segment_fault(){
    printf("***stack segment fault***\n");

    while (1);

    return 0;
};

int general_protection(){
    printf("general protection fault\n");
    
    while (1);

    return 0;
};

int page_fault(){
    printf("page fault occurred\n");

    while (1);

    return 0;
};

int x87_fpu_floating_point_error(){
    printf("x87 fpu floating point error occurred\n");

    while (1);

    return 0;
}

int alignment_check(){
    printf("alignment check interrupt occurred\n");

    while (1);

    return 0;
}
int machine_alignment(){
    printf("machine alignment interrupt occurred\n");

    while (1);

    return 0;
}
int simd_floating_point_exception(){
    printf("simd floating point exception occurred\n");

    while (1);

    return 0;
}
int user_defined(){
    printf("user defined interrupt occurred\n");

    while (1);

    return 0;
}

int system_call() {
    printf("system call occurred\n");

    while (1);

    return 0;
}
