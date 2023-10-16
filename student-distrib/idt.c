#include "idt.h"
#include "exceptions.h"
#include "asm_linkage.h"
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

/* 
 * init_idt
 *   DESCRIPTION: Initializes the interrupt descriptor table with exceptions (first 20 entries),
 *                keyboard, RTC, and system call (0x80).
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: adds entries to IDT
 */
void init_idt() {
    // TODO: set up interrupt wrapper (look at ece391syscall.S)
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
            idt[i].present = 1;
            idt[i].dpl = 0;
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 1;
            idt[i].seg_selector = KERNEL_CS;
        } else if (i == 0x80) {
            idt[i].present = 1;
            idt[i].dpl = 3; // set privilege level 1
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 0;
            idt[i].seg_selector = KERNEL_CS;
        } else if (i == 0x21 || i == 0x28) {
            idt[i].present = 1;
            idt[i].dpl = 0; // set privilege level 1
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 0;
            idt[i].seg_selector = KERNEL_CS;
        } else {
            idt[i].present = 0;
            idt[i].dpl = 0; // set privilege level 1
            idt[i].reserved0 = 0;
            idt[i].size = 1; // size of gate - INT gate is a 32 bit gate
            idt[i].reserved1 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved3 = 0;
            idt[i].seg_selector = KERNEL_CS;
        }
    }

    /* instantiate the kernel function pointers */
    // Setting intel defined entries
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
    idt[0x0F].reserved3 = 0;
    SET_IDT_ENTRY(idt[0x0F], reserved);
    SET_IDT_ENTRY(idt[0x10], x87_fpu_floating_point_error);
    SET_IDT_ENTRY(idt[0x11], alignment_check);
    SET_IDT_ENTRY(idt[0x12], machine_check);
    SET_IDT_ENTRY(idt[0x13], simd_floating_point_exception);
    
    // Keyboard PIC interupt
    SET_IDT_ENTRY(idt[0x21], keyboard_handler_linkage);

    // RTC PIC Intertupt
    SET_IDT_ENTRY(idt[0x28], rtc_handler_linkage); // PIC INT call
    
    SET_IDT_ENTRY(idt[0x80], system_call); // INT system call    
}
