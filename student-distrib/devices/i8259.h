/* i8259.h - Defines used in interactions with the 8259 interrupt
 * controller
 * vim:ts=4 noexpandtab
 */

#ifndef _I8259_H
#define _I8259_H

#include "../types.h"
#include "../lib.h"
#include "../x86_desc.h"

/* Ports that each PIC sits on */
#define MASTER_8259_PORT    0x20
#define SLAVE_8259_PORT     0xA0
#define PIC1_COMMAND	MASTER_8259_PORT
#define PIC1_DATA	    (MASTER_8259_PORT+1)
#define PIC2_COMMAND	SLAVE_8259_PORT
#define PIC2_DATA	    (SLAVE_8259_PORT+1)

/* keyboard and RTC port*/
#define KEYBOARD_PORT       0x60


/* Initialization control words to init each PIC.
 * See the Intel manuals for details on the meaning
 * of each word */
#define ICW1_INIT           0x10
#define ICW1                0x11
#define ICW2_MASTER         0x20
#define ICW2_SLAVE          0x28
#define ICW3_MASTER         0x04
#define ICW3_SLAVE          0x02
#define ICW4                0x01

/* End-of-interrupt byte.  This gets OR'd with
 * the interrupt number and sent out to the PIC
 * to declare the interrupt finished */
#define EOI                 0x60

/* Externally-visible functions */

/* Initialize both PICs */
void i8259_init(void);
/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num);
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num);
/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num);
/* offset pic vector and set up primary/secondary cascade */ //TODO : check definition
void PIC_remap(int offset1, int offset2);
//int int handle_PIC_trash();

//https://wiki.osdev.org/8259_PIC
//https://forum.osdev.org/viewtopic.php?f=1&t=9746&sid=1847604ca981b6bbc22c6e8f9e97e7da&start=15
//https://wiki.osdev.org/8259_PIC#The_IBM_PC_8259_PIC_Architecture
//https://courses.engr.illinois.edu/ece391/fa2023/secure/references/IA32-ref-manual-vol-3.pdf
//https://stackoverflow.com/questions/37618111/keyboard-irq-within-an-x86-kernel ***********************************************************



#endif /* _I8259_H */
