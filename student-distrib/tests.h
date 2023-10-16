#ifndef TESTS_H
#define TESTS_H


// test launcher
void launch_tests();
int idt_test_handler(int test_num);

/* Interrupt Test Functions */
int test_divide_error();
int test_reserved_interupt();
int test_bound_range_exceeded();

int test_NMI_interupt();
int test_breakpoint();
int test_overflow_interrupt();
int test_invalid_opcode();
int test_device_not_available();
int test_double_fault();
int test_segment_overrun();
int test_invalid_tss();
int test_segment_not_present();
int test_stack_fault();
int test_general_protection();
int test_page_fault();
int test_x86_FPU_error();
int test_alignment_check();
int test_machine_check();
int test_simd_floating_point_exception();

int test_syscall_handler();
int test_paging();
int test_page_fault_handler();
int test_null();

#endif /* TESTS_H */
