#include "x86_desc.h"
#include "devices/keyboard.h"

<<<<<<< HEAD
// Intializes our interupt descriptor table
int idt_init();

// Sets the interupts on the table
=======
int init_idt(); // TODO: comment
>>>>>>> f4c77c3250dc8ebe4b67dc923d1bdb4fbacff98a
void set_idt_interrupt(int index, void *f); // TODO: @ page 148, reports return adddress for Faults and Traps

// Setting up the traps
void set_idt_trap(int index, void *f);
