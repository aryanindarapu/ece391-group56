/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "keyboard.h"
#include "rtc.h"
#include "pit.h"
#include "../lib.h"
#include "../x86_desc.h"




/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/*
 *   i8259_init
 *   DESCRIPTION: initializes the PIC by setting masks and calling helper function to map it to idt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Prepares pic for operation and intializes starting state
 */  

void i8259_init(void) {
    // look pg 795-796
    
    // Include master_mask and slave_mask to perform bitwise ops
    master_mask = 0xFF;
    slave_mask = 0xFF;

    // Offset primary PIC to 0x20 on IDT, offset secondary PIC to 0x28 on IDT
	PIC_remap(ICW2_MASTER, ICW2_SLAVE);
	
	// Other devices are initialized in helper functions after 8259
    init_keyboard();
    init_rtc();
    init_pit();
}



/*
 *   enable_irq
 *   DESCRIPTION: Enables the irq bit on the PIC and updates masks
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: updates master_mask and slave_mask as well as PIC internal state
 */  
void enable_irq(uint32_t irq_num) {
    // Check if the interrupt is occurring on the second or the first pic
    // Each PIC has 8 IRQs so IRQs 0-7 are primary, 8-15 are secondary
    if (irq_num < 8) {
        // Enable normally for primary pic
        master_mask &= (~(1 << irq_num));
        outb(master_mask,PIC1_DATA);
    } else {
        // Subtract 8 for secondary since we still index on the PIC2_DATA from 0-7
        slave_mask &= (~(1 << (irq_num - 8)));
        outb(slave_mask,PIC2_DATA);
    }
    return;
}

/*
 *   disable_irq
 *   DESCRIPTION: Disables the irq bit on the PIC and updates masks
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: updates master_mask and slave_mask as well as PIC internal state
 */  
void disable_irq(uint32_t irq_num) {
    // Same thing as the enable except we OR it with the mask to disable the interrupt
    // This will make that corresponding IRQ disabled
    if (irq_num < 8) {
        master_mask |= (~(1 << irq_num));
        outb(master_mask, PIC1_DATA);
    } else {
        slave_mask |= (~(1 << (irq_num - 8)));
        outb(slave_mask, PIC2_DATA);
    }
    return;
}

/*
 *   send_eoi
 *   DESCRIPTION: Sends the EOI to the corresponding irq on the PIC
 *   INPUTS: irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: updates PIC internal state to tell it to stop the interrupt
 */  
void send_eoi(uint32_t irq_num) {
    // Check if the irq is from the secondary or primary
    if (irq_num >= 8) {
        // Have to send EOI to primary and secondary to let them know interrupt is over
        outb(EOI | (irq_num - 8), PIC2_COMMAND);
        // Then we have to send it to the primary
        // We or it with two since it comes from the secondary PIC
        outb(EOI | 0x02, PIC1_COMMAND);
    } else {
        // Came from primary with no secondary so just let primary PIC know
        outb(EOI | irq_num, PIC1_COMMAND);
    }   
}

/*
 *   PIC_remap
 *   DESCRIPTION: This is a helper function for the initialization process
 *   INPUTS: offset1, offset2
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: updates PIC internal state that starts the pic and sets the vector offsets to the IDT, as well as CASC info for secondary and primary
 */  
void PIC_remap(int offset1, int offset2)
{

	outb(ICW1_INIT | ICW4, PIC1_COMMAND);  // starts the initialization sequence (in cascade mode)
	outb(ICW1_INIT | ICW4, PIC2_COMMAND);
	outb(offset1, PIC1_DATA);                 // ICW2: Master PIC vector offset
	outb(offset2, PIC2_DATA);                 // ICW2: Slave PIC vector offset
	outb(ICW3_MASTER, PIC1_DATA);             // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(ICW3_SLAVE, PIC2_DATA);              // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	outb(ICW4, PIC1_DATA);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	outb(ICW4, PIC2_DATA);
}

