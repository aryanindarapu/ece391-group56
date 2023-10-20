#ifndef TESTS_H
#define TESTS_H


// test launcher
void launch_tests();
int idt_test_handler(int test_num);

/* Interrupt Test Functions */
int test_divide_error();
int test_reserved_interupt();
int test_bound_range_exceeded();

int test_syscall_handler();
int test_paging();
int test_page_fault_handler();
int test_null();

/* Checkpoint 2 Tests */
int test_file();

#endif /* TESTS_H */
