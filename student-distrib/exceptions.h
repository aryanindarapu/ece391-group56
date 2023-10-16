#include "lib.h"

/* Handlers for exceptions from 0x00 to 0x13 */
void divide_error();
void debug(); 
void nmi_interrupt();
void breakpoint();
void overflow();
void bound_range_exceeded();
void invalid_opcode();
void device_not_available();
void double_fault();
void coprocessor_segment_overrun();
void invalid_tss();
void segment_not_present();
void stack_segment_fault();
void general_protection();
void page_fault();
void reserved();
void x87_fpu_floating_point_error();
void alignment_check();
void machine_check();
void simd_floating_point_exception();
void user_defined();


/* Handler for system call interrupt (0x80) */
void system_call();
