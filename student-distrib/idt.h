#include "x86_desc.h"

int idt_init(); // TODO: comment
void set_idt_interrupt(int index, int (*f)()); // TODO: @ page 148, reports return adddress for Faults and Traps
void set_idt_trap(int index, int (*f)());
