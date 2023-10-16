#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#define PASS 1
#define FAIL 0
#define TEST_VECTOR 0 // Set to IDT vector number OR to 256 for keyboard tests

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// TODO: comment AND change to TEST_OUTPUT type function
int test_divide_error() {
	TEST_HEADER;
	int a, b, c;
	a = 10;
	b = 0;
	c = a / b;
	return FAIL;
}

int test_syscall_handler() {
	TEST_HEADER;
	asm volatile("int $0x80");
	return FAIL;
}

int test_paging() {
	TEST_HEADER;
	int a;
	int* p;

    printf("Point to beginning of kernel memory.\n");
    p = (int *) 0x400000; // point to kernel memory
    a = *p;
    printf("PASSED\n");

    printf("Pointing to end of kernel memory.\n");
    p = (int *) 0x700000; // point to kernel memory
    a = *p;
    printf("PASSED\n");

    printf("Pointing to beginning of video memory.\n");
    p = (int *) 0xB8000; // point to kernel memory
    a = *p;
    printf("PASSED\n");

	// TODO: fix this test, it still doesn't work
    // printf("Pointing to end of video memory.\n");
    // p = (int *) 0xB8FFd; // point to kernel memory
    // a = *p;
    // printf("PASSED\n");

	return PASS;
}

int test_page_fault_handler() {
	TEST_HEADER;
	int a;
	int* p;

    printf("Point to beginning of kernel memory.\n");
    p = (int *) 0x400000; // point to kernel memory
    a = *p;
    printf("PASSED\n");

    printf("Pointing to not present memory.\n");
    p = (int *) 0x2; // point to kernel memory
    a = *p;
    printf("FAILED\n");

	// TODO: fix this test, it still doesn't work
    // printf("Pointing to end of video memory.\n");
    // p = (int *) 0xB8FFd; // point to kernel memory
    // a = *p;
    // printf("PASSED\n");

	return FAIL;
}


int test_null() {
	TEST_HEADER;
	int a;
    int* p;

    p = (int *) 0x2; 
	a = *p;

	return FAIL;
}

// TODO: add more tests for each interrupt

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests() {
	clear();
	// TEST_OUTPUT("idt_test", idt_test());
	TEST_OUTPUT("General IDT Test", idt_test());
	TEST_OUTPUT("Paging Test", test_paging());
	TEST_OUTPUT("NULL Dereference Test", test_null());
	// TEST_OUTPUT("idt_test", test_page_fault_handler());
}
