#include "lib.h"

int divide_error();
int nmi_interrupt();
int breakpoint();
int overflow();
int bound_range_exceeded();
int invalid_opcode();
int device_not_available();
int double_fault();
int coprocessor_segment_overrun();
int invalid_tss();
int segment_not_present();
int stack_segment_fault();
int general_protection();
int page_fault();
int x87_FPU_floating_point_error();
int alignment_check();
int machine_alignment();
int simd_floating_point_exception();
int user_defined();

int system_call();
