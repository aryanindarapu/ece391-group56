#include "interrupt_handlers.h"

int divide_error() {
    // TODO: do a while 1 loop
    while (1) {
        printf("divide error occurred\n");
    }
    
    return 0;
}

int nmi_interrupt() {
    printf("nmi innterrupt occured\n");
    return 0;
};

int breakpoint() {
    printf("breakpoint interrupt occurred\n");
    return 0;
};

int overflow() {
    printf("overflow error occurred\n");
    return 0;
};

int bound_range_exceeded() {
    printf("bound range error occurred\n");
    return 0;
};

int invalid_opcode() {
    printf("invalid opcode error occurred\n");
    return 0;
};

int device_not_available(){
    printf("device not available\n");
    return 0;
};

int double_fault(){
    printf("double fault error occurred\n");
    return 0;
};

int coprocessor_segment_overrun(){
    printf("coprocessor segment overrun error occurred\n");
    return 0;
};

int invalid_tss(){
    printf("invalid tts error occurred\n");
    return 0;
};

int segment_not_present(){
    printf("***segment not present***\n");
    return 0;
};

int stack_segment_fault(){
    printf("***stack segment fault***\n");
    return 0;
};

int general_protection(){
    printf("general protection fault\n");
    return 0;
};

int page_fault(){
    printf("page fault occurred\n");
    return 0;
};

int system_call() {
    printf("system call occurred\n");
    return 0;
}
