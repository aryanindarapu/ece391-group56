#include "x86_desc.h"
#include "devices/keyboard.h"

int init_idt(); // TODO: comment
void set_idt_interrupt(int index, void *f); // TODO: @ page 148, reports return adddress for Faults and Traps
void set_idt_trap(int index, void *f);
