#include "idt.h"
#include "interrupt_handlers.h"
#include "lib.h" // for printing


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

void set_idt_interrupt(int index, void (*f)(int)) {
    {
        idt_desc_t idt_descriptor;
        idt_descriptor.present = 1;
        idt_descriptor.dpl = 0; // No idea what this should be, but may be 0
        idt_descriptor.reserved0 = 0;
        idt_descriptor.size = 1; // size of gate - INT gate is a 32 bit gate
        idt_descriptor.reserved1 = 1;
        idt_descriptor.reserved2 = 1;
        idt_descriptor.reserved3 = 0;
        idt_descriptor.seg_selector = KERNEL_CS; // is this correct val?
        SET_IDT_ENTRY(idt_descriptor, f);

        idt[index] = idt_descriptor;
    }
}

// int idt_init() { // TODO: change to init_idt
//     // TODO: set up IRQ for everything here
//     // TODO: how do I separate the IRQ, TRAP, TASK?
//     // SET_IDT_ENTRY(idt[vector number], func name);


//     // Setting up for divide error - Interrupt gate
//     // .offset_15_00 = 0;
//     printf("reached idt init\n");


//     int i;
//     for (i = 0; i < NUM_VEC; i++) {
        
//         switch (i) {
//             case 0:
//                 // set_idt_interrupt(i, divide_error);
//                 break;
//             // case 1: //RESERVED
//             //     break;
//             // case 2: 
//             //     nmi_interrupt();
//             //     break;
//             // case 3:
//             //     breakpoint();
//             //     break;
//             // case 4: 
//             //     overflow();
//             //     break;
//             // case 5:
//             //     bound_range_exceeded();
//             //     break;
//             // case 6:
//             //     invalid_opcode();
//             //     break;
//             // case 7:
//             //     device_not_available();
//             //     break;
//             // case 8:
//             //     double_fault();
//             //     break;
//             // case 9:
//             //     coprocessor_segment_overrun();
//             //     break;
//             // case 10:
//             //     invalid_tts();
//             //     break;
//             // case 11:
//             //     segment_not_present();
//             //     break;
//             // case 12:
//             //     stack_segment_fault();
//             //     break;
//             // case 13:
//             //     general_protection();
//             //     break;
//             // case 14:
//             //     page_fault();
//             //     break;
//             // case 15:
//             //     //RESERVED
//             //     break;
//             // case 16:
//             //     x87_FPU_floating_point_error();
//             //     break;
//             // case 17:
//             //     alignment_check();
//             //     break;
//             // case 18:
//             //     machine_alignment();
//             //     break;
//             // case 19:
//             //     simd_floating_point_exception();
//             //     break;
//             // case 32: 
//             //     user_defined();
//             //     idt[i].dpl = 3; /* THIS IS USER LEVEL CODE SO ITS A LOWER PRIORITY */
//             //     break;
//             default: 
//             // {
//             //     idt_descriptor.present = 1;
//             //     idt_descriptor.dpl = 0; // No idea what this should be, but may be 0
//             //     idt_descriptor.reserved0 = 0;
//             //     idt_descriptor.size = 1; // size of gate - INT gate is a 32 bit gate
//             //     idt_descriptor.reserved1 = 1;
//             //     idt_descriptor.reserved2 = 1;
//             //     idt_descriptor.reserved3 = 0;
//             //     idt_descriptor.seg_selector = KERNEL_DS; // is this correct val?
//             //     SET_IDT_ENTRY(idt_descriptor, divide_error);
//             // }
//                 break;
//         }

//         // idt[i] = idt_descriptor;
//     }


//     // eventually, iterate through 256 entries and call SET_IDT_ENTRY using assembly jump table wrapper
//     return 0;
// }

int idt_init() {
    int i, j;
    for (i = 0; i < NUM_VEC; i++){
        for (j = 0; j < 32; j++){ //all the kernel interrupts 

        }
    }
    return 0;
}
