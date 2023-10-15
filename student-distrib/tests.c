#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#define PASS 1
#define FAIL 0
#define TEST_VECTOR 0 // 

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	// asm volatile("int $15");
	asm volatile("int $128");
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
	int a, b, c;
	a = 10;
	b = 0;
	c = a / b;
	return 0;
}


int test_page_fault() {
    //Used to test dereference locations.
	int a;
    int* p = (int *) 0x400000; // point to kernel memory
    a = *p;

	printf("pointing to kernel memory");

    p = (int *) 0x2; 
	a = *p;
}

// TODO: add more tests for each interrupt

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests() {
	// TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	clear();
	switch (TEST_VECTOR) {
		case 0:
			test_divide_error();
			break;
		case 14:
			test_page_fault();
			break;
		case 33:
			asm volatile("int $33");
			break;
		case 40:
			asm volatile("int $40");
			break;
		case 128:
			asm volatile("int $128");
			break;
		default:
			break;
	}
}
