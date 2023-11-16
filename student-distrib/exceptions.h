#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include "lib.h"

/* Handlers for exceptions from 0x00 to 0x13 */
int divide_error();
int debug(); 
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
int reserved();
int x87_fpu_floating_point_error();
int alignment_check();
int machine_check();
int simd_floating_point_exception();
int user_defined();

int exception_raised_flag;
#endif
