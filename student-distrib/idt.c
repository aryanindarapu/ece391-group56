#include "idt.h"
#include "interrupt_handlers.h"
#include "lib.h" // for printing

// int (*FUNCTION_POINTERS[NUM_VEC])(); // Array of function pointers to handlers


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

// void set_idt_interrupt(int index, int (*f)()) {
//     {
//         idt_desc_t idt_descriptor;
//         idt_descriptor.present = 1;
//         idt_descriptor.dpl = 0; // TODO: figure out what these privilege levels should be
//         idt_descriptor.reserved0 = 0;
//         idt_descriptor.size = 1; // size of gate - INT gate is a 32 bit gate
//         idt_descriptor.reserved1 = 1;
//         idt_descriptor.reserved2 = 1;
//         idt_descriptor.reserved3 = 0;
//         idt_descriptor.seg_selector = KERNEL_CS;
//         SET_IDT_ENTRY(idt_descriptor, f);
//
//         idt[index] = idt_descriptor;
//     }
// }

// void set_idt_trap(int index, int (*f)()) {
//     {
//         idt_desc_t idt_descriptor;
//         idt_descriptor.present = 1;
//         idt_descriptor.dpl = 0; // TODO: double check dpl
//         idt_descriptor.reserved0 = 0;
//         idt_descriptor.size = 1; // size of gate - INT gate is a 32 bit gate
//         idt_descriptor.reserved1 = 1;
//         idt_descriptor.reserved2 = 1;
//         idt_descriptor.reserved3 = 1;
//         idt_descriptor.seg_selector = KERNEL_CS; // is this correct val?
//         SET_IDT_ENTRY(idt_descriptor, f);
//
//         idt[index] = idt_descriptor;
//     }
// }

int idt_init() { // TODO: change to init_idt
    // TODO: set up IRQ for everything here
    // TODO: how do I separate the IRQ, TRAP, TASK?
    // SET_IDT_ENTRY(idt[vector number], func name);
    
    /* instantiate the kernel function pointers */
    SET_IDT_ENTRY(idt[0x00], divide_error);
    SET_IDT_ENTRY(idt[0x01], debug);
    SET_IDT_ENTRY(idt[0x02], nmi_interrupt);
    SET_IDT_ENTRY(idt[0x03], breakpoint);
    SET_IDT_ENTRY(idt[0x04], overflow);
    SET_IDT_ENTRY(idt[0x05], bound_range_exceeded);
    SET_IDT_ENTRY(idt[0x06], invalid_opcode);
    SET_IDT_ENTRY(idt[0x07], device_not_available);
    SET_IDT_ENTRY(idt[0x08], double_fault);
    SET_IDT_ENTRY(idt[0x09], coprocessor_segment_overrun);
    SET_IDT_ENTRY(idt[0x0A], invalid_tss);
    SET_IDT_ENTRY(idt[0x0B], segment_not_present);
    SET_IDT_ENTRY(idt[0x0C], stack_segment_fault);
    SET_IDT_ENTRY(idt[0x0D], general_protection);
    SET_IDT_ENTRY(idt[0x0E], page_fault);
    SET_IDT_ENTRY(idt[0x0F], reserved);
    SET_IDT_ENTRY(idt[0x10], x87_fpu_floating_point_error);
    SET_IDT_ENTRY(idt[0x11], alignment_check);
    SET_IDT_ENTRY(idt[0x12], machine_check);
    SET_IDT_ENTRY(idt[0x13], simd_floating_point_exception);

    SET_IDT_ENTRY(idt[0x80], system_call); // INT system call

    int i;
    for (i = 0; i < NUM_VEC; i++) {
        if (i < 20) {
            idt[i].present = 1;
            idt[i].dpl = 0; // set privilege level 0
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 1;
            idt[i].seg_selector = KERNEL_CS;
        } else if (i < 32) {
            idt[i].present = 0;
            idt[i].dpl = 0;
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 1;
            idt[i].seg_selector = KERNEL_CS;
        } else if (i == 0x80) {
            idt[i].present = 1;
            idt[i].dpl = 1; // set privilege level 1
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 0;
            idt[i].seg_selector = KERNEL_CS;
        } else {
            idt[i].present = 0;
            idt[i].dpl = 1; // set privilege level 1
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 0;
            idt[i].seg_selector = KERNEL_CS;
        }
    }

    /*
    FUNCTION_POINTERS[0] = divide_error;

    FUNCTION_POINTERS[2] = nmi_interrupt;
    FUNCTION_POINTERS[3] = breakpoint;
    FUNCTION_POINTERS[4] = overflow;
    FUNCTION_POINTERS[5] = bound_range_exceeded;
    FUNCTION_POINTERS[6] = invalid_opcode;
    FUNCTION_POINTERS[7] = device_not_available;
    FUNCTION_POINTERS[8] = double_fault;
    FUNCTION_POINTERS[9] = coprocessor_segment_overrun;
    FUNCTION_POINTERS[10] = invalid_tss;
    FUNCTION_POINTERS[11] = segment_not_present;
    FUNCTION_POINTERS[12] = stack_segment_fault;
    FUNCTION_POINTERS[13] = general_protection;
    FUNCTION_POINTERS[14] = page_fault;

    FUNCTION_POINTERS[16] = x87_fpu_floating_point_error;
    FUNCTION_POINTERS[17] = alignment_check;
    FUNCTION_POINTERS[18] = machine_alignment;
    FUNCTION_POINTERS[19] = simd_floating_point_exception;
    // TODO this is user defined interrupts so its not necessarily idt[32]
    FUNCTION_POINTERS[32] = user_defined; 

    FUNCTION_POINTERS[128] = system_call; // INT system call
    */

    // Setting up for divide error - Interrupt gate
    // .offset_15_00 = 0;
    printf("reached idt init\n");

    // int i;
    // for (i = 0; i < NUM_VEC; i++) {
    //     switch (i) {
    //         case 0:
    //             set_idt_interrupt(i, divide_error);
    //             break;
    //         case 1: // TODO: add exception here
    //             break;
    //         case 2:
    //             set_idt_interrupt(i, nmi_interrupt);
    //             break;
    //         case 3:
    //             set_idt_trap(i, breakpoint);
    //             break;
    //         case 4:
    //             set_idt_trap(i, overflow);
    //             break;
    //         case 5:
    //             set_idt_interrupt(i, bound_range_exceeded);
    //             break;
    //         case 6:
    //             set_idt_interrupt(i, invalid_opcode);
    //             break;
    //         case 7:
    //             set_idt_interrupt(i, device_not_available);
    //             break;
    //         case 8: //THIS IS SUPPOSED TO BE AN ABORT NOT INTER?
    //             set_idt_interrupt(i, double_fault);
    //             break;
    //         case 9:
    //             set_idt_interrupt(i, coprocessor_segment_overrun);
    //             break;   
    //         case 10:
    //             set_idt_interrupt(i, invalid_tss);
    //             break;
    //         case 11:
    //             set_idt_interrupt(i, segment_not_present);
    //             break;
    //         case 12:
    //             set_idt_interrupt(i, stack_segment_fault);
    //             break;
    //         // case 13:
    //         //     set_idt_interrupt(i, FUNCTION_POINTERS[i]);
    //         //     break;
    //         // case 14:
    //         //     set_idt_interrupt(i, FUNCTION_POINTERS[i]);
    //         //     break;
    //         // case 15:
    //         //     break;
    //         // case 16:
    //         //     set_idt_interrupt(i, FUNCTION_POINTERS[i]);
    //         //     break;
    //         // case 17:
    //         //     set_idt_interrupt(i, FUNCTION_POINTERS[i]);
    //         //     break;
    //         // case 18: //THIS IS SUPPOSED TO BE AN ABORT NOT INTER?
    //         //     set_idt_interrupt(i, FUNCTION_POINTERS[i]);
    //         //     break;
    //         // case 19:
    //         //     set_idt_interrupt(i, FUNCTION_POINTERS[i]);
    //         //     break;
    //         default:
    //             {
    //                 idt_desc_t idt_descriptor;
    //                 idt[i] = idt_descriptor;
    //             }
    //             break;
    //     }
    // }    

    // eventually, iterate through 256 entries and call SET_IDT_ENTRY using assembly jump table wrapper
    return 0;
}
