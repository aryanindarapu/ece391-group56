#include "lib.h"

int get_line_number();

void divide_error();
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
void x87_fpu_floating_point_error();
void alignment_check();
void machine_alignment();
void simd_floating_point_exception();
void user_defined();

void system_call();
