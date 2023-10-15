#ifndef TESTS_H
#define TESTS_H


// test launcher
void launch_tests();
int idt_test_handler(int test_num);

/* Interrupt Test Functions */
int test_divide_error();

#endif /* TESTS_H */
