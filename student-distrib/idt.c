#include "idt.h"
#include "lib.h" // for printing

int divideError() {
    // TODO: do a while 1 loop
    printf("divide error occurred");
    return 0;
}

int syscall() {
    printf("at syscall, somehow");
    return 0;
}

/* NOTE: IDT DPL levels - @ page 113
 * 0 - OS kernel
 * 1 & 2 - OS Services
 * 3 - Applications
 * 
 * Segment Selector
 * - Bits 15 to 3: Bits 3 - 15 of Index of the GDT or LDT entroy
 * - Bit 2: Specifies descriptor table to use
 * - Bits 1 to 0: Requested Privilege Level (@ page 113) 
*/

int idt_init(idt_desc_t* idt) {
    // TODO: set up IRQ for everything here
    // TODO: how do I separate the IRQ, TRAP, TASK?
    // SET_IDT_ENTRY(idt[vector number], func name);


    // Setting up for divide error - Interrupt gate
    // .offset_15_00 = 0;
    printf("reached idt init");
    SET_IDT_ENTRY(idt[0x00], divideError);
    // idt[0x00].seg_selector = idk
    idt[0x00].present = 1;
    idt[0x00].dpl = 0; // No idea what this should be, but may be 0
    idt[0x00].reserved0 = 0;
    idt[0x00].size = 1; // size of gate - INT gate is a 32 bit gate
    idt[0x00].reserved1 = 1;
    idt[0x00].reserved2 = 1;
    idt[0x00].reserved3 = 0;
    idt[0x00].seg_selector = KERNEL_CS; // is this correct val?
    // eventually, iterate through 256 entries and call SET_IDT_ENTRY using assembly jump table wrapper

}