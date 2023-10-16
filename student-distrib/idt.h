#include "x86_desc.h"
#include "devices/keyboard.h"

// Intializes our interupt descriptor table
int idt_init();

// Sets the interupts on the table
void set_idt_interrupt(int index, void *f); // TODO: @ page 148, reports return adddress for Faults and Traps

// Setting up the traps
void set_idt_trap(int index, void *f);
