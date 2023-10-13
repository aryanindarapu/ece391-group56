/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    // TODO: initialize devices

    master_mask = 0x00;
    slave_mask = 0x00;
    /*
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
    
    
    
    
    
    */
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    master_mask = master_mask & (1<<irq_num);
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    master_mask = master_mask & (0xffff-(1<<irq_num));
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    outb(EOI | irq_num, MASTER_8259_PORT);
}
