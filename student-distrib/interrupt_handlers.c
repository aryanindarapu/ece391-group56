#include "interrupt_handlers.h"
#include "i8259.h"

void divide_error() {
    printf("divide error occurred\n");
    while (1);    
}

void debug() {
    printf("debug interrupt occurred\n");
    while (1);
}

void nmi_interrupt() {
    printf("nmi innterrupt occured\n");
    while (1);
}

void breakpoint() {
    printf("breakpoint interrupt occurred\n");
    while (1);
}

void overflow() {
    printf("overflow error occurred\n"); 
    while (1);
}

void bound_range_exceeded() {
    printf("bound range error occurred\n");
    while (1);
}

void invalid_opcode() {
    printf("invalid opcode error occurred\n");
    while (1);
    
}

void device_not_available() {
    printf("device not available\n");
    while (1);
}

void double_fault() {
    printf("double fault error occurred\n");
    while (1);
}

void coprocessor_segment_overrun() {
    printf("coprocessor segment overrun error occurred\n");
    while (1);
}

void invalid_tss() {
    printf("invalid tts error occurred\n");
    while (1);  
}

void segment_not_present() {
    printf("***segment not present***\n");
    while (1);
}

void stack_segment_fault() {
    printf("***stack segment fault***\n");
    while (1);
}

void general_protection() {
    printf("general protection fault\n");
    while (1);
}

void page_fault() {
    printf("page fault occurred\n");
    while (1);
}

void reserved() {
    printf("reserved interrupt occurred\n");
    while (1);
}

void x87_fpu_floating_point_error() {
    printf("x87 fpu floating point error occurred\n");
    while (1);
}

void alignment_check() {
    printf("alignment check interrupt occurred\n");
    while (1);
}

void machine_check() {
    printf("machine alignment interrupt occurred\n");
    while (1);
}

void simd_floating_point_exception() {
    printf("simd floating point exception occurred\n");
    while (1);
}

void user_defined() {
    printf("user defined interrupt occurred\n");
    while (1);
}

void system_call() {
    printf("system call occurred\n");
    while (1);   
}

void read_keyboard() {
    
    //printf("keyboard int occured\n");
    unsigned char status;
    char keycode;
    
    unsigned char keyboard_map[128] =
    {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
    '9', '0', '-', '=', '\b',     /* Backspace */
    '\t',                 /* Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
        0,                  /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',     /* 39 */
    '\'', '`',   0,                /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
    'm', ',', '.', '/',   0,                              /* Right shift */
    '*',
        0,  /* Alt */
    ' ',  /* Space bar */
        0,  /* Caps lock */
        0,  /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  /* < ... F10 */
        0,  /* 69 - Num lock*/
        0,  /* Scroll Lock */
        0,  /* Home key */
        0,  /* Up Arrow */
        0,  /* Page Up */
    '-',
        0,  /* Left Arrow */
        0,
        0,  /* Right Arrow */
    '+',
        0,  /* 79 - End key*/
        0,  /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        0,  /* Delete Key */
        0,   0,   0,
        0,  /* F11 Key */
        0,  /* F12 Key */
        0,  /* All other keys are undefined */
    };
    //status = inb(0x64);
    
    /* Lowest bit of status will be set if buffer is not empty */
    //if (status & 0x01) {
        keycode = inb(0x60);
        if(keycode > 0)
            printf("%c", keyboard_map[keycode]);
  //  }
    //printf("keyboard int ended\n");
    send_eoi(1);
    // send EOI below 
    
}
