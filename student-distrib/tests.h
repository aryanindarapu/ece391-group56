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
#define FRAME1_SIZE 174
#define HELLO_SIZE 5349
#define VERYLARGE_SIZE 5277

int test_frame1();
int test_hello();
int test_verylarge();
int test_directory_ls();

int test_rtc_driver();
int test_terminal_driver();

void stdin(char* buf);
void stdout(char* buf);

#endif /* TESTS_H */
