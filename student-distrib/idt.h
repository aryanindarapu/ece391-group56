#include "x86_desc.h"

int idt_init(); // TODO: comment
void set_idt_interrupt(int index, void (*f)(int));
