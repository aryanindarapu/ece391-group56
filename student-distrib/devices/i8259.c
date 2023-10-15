/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"




/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    // TODO: initialize devices
    // look pg 795-796
    
    // Include master_mask and salve_mask to perform bitwise ops
    master_mask = 0xFF;
    slave_mask = 0xFF;

    // Offset primary PIC to 0x20 on IDT, offset secondary PIC to 0x28 on IDT
	PIC_remap(ICW2_MASTER, ICW2_SLAVE);
	
	// Other devices are initialized in helper functions after 8259
}



/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    //master_mask = master_mask & (1<<irq_num); // TODO: Explain/check
	// uint16_t port;
    // uint8_t value;
 
    // if(irq_num < 8) {
    //     port = PIC1_DATA;
    // } else {
    //     port = PIC2_DATA;
    //     irq_num -= 8;
    // }
    // value = inb(port) & ~(1 << irq_num);
    // outb(value, port);  
    if (irq_num < 8) {
        master_mask &= (~(1 << irq_num));
        outb(master_mask,PIC1_DATA);
    } else {
        slave_mask &= (~(1 << (irq_num - 8)));
        outb(slave_mask,PIC2_DATA);
    }
    return;
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    //master_mask = master_mask & (0xffff-(1<<irq_num)); // TODO: Explain/check
	// uint16_t port;
    // uint8_t value;
 
    // if(irq_num < 8) {
    //     port = PIC1_DATA;
    // } else {
    //     port = PIC2_DATA;
    //     irq_num -= 8;
    // }
    // value = inb(port) | (1 << irq_num);
    // outb(value, port);
    if (irq_num < 8) {
        master_mask |= (~(1 << irq_num));
        outb(master_mask, PIC1_DATA);
    } else {
        slave_mask |= (~(1 << (irq_num - 8)));
        outb(slave_mask, PIC2_DATA);
    }
    return;
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    // TODO: Check the irq_num to see if it is greater than 8, that means we have to go to the second PIC
    //outb(EOI | irq_num, MASTER_8259_PORT); // TODO: Explain/check
    if (irq_num >= 8) {
        
        outb(EOI | (irq_num - 8), PIC2_COMMAND);
        outb(EOI | 0x02, PIC1_COMMAND); // TODO: comment on hard coded x02
    } else {
        //printf("in EOI %d\n", irq_num);
        outb(EOI | irq_num, PIC1_COMMAND);
        //printf("finish EOI\n");
    }   
}

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	// unsigned char a1, a2;
 
	// a1 = inb(PIC1_DATA);                        // save masks
	// a2 = inb(PIC2_DATA);
 
	outb(ICW1_INIT | ICW4, PIC1_COMMAND);  // starts the initialization sequence (in cascade mode)
	outb(ICW1_INIT | ICW4, PIC2_COMMAND);
	outb(offset1, PIC1_DATA);                 // ICW2: Master PIC vector offset
	outb(offset2, PIC2_DATA);                 // ICW2: Slave PIC vector offset
	outb(ICW3_MASTER, PIC1_DATA);             // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(ICW3_SLAVE, PIC2_DATA);              // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	outb(ICW4, PIC1_DATA);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	outb(ICW4, PIC2_DATA);
 
	// outb(a1, PIC1_DATA);   // restore saved masks.
	// outb(a2, PIC2_DATA);
	//printf("remap done\n");
}


// int handle_PIC_trash() {
//     printf("in pic :)\n");
//     for (;;) {
//         char c=inb(60);
//         if (!(c&80)) // don't print break codes
//             printf((char)c);
//         }
//     }
    

//     return 0;
// };

