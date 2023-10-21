#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "devices/rtc.h"
#include "terminal.h"

#define PASS 1
#define FAIL 0

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

/*
 *   test_divide_error
 *   DESCRIPTION: Attempt to divide by 0
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Causes a divide by 0 error
 */ 
int test_divide_error() {
	TEST_HEADER;
	int a, b, c;
	a = 10;
	b = 0;
	c = a / b;
	return FAIL;
}

/*
 *   reservd_interupt
 *   DESCRIPTION: Intel Reserved interupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Calls on our IDT
 */ 
int test_reserved_interupt(){
	// Use #1 for reserved interupt
	TEST_HEADER;
	asm volatile("int $1");
	return FAIL;
}

/*
 *   NMI Interupt
 *   DESCRIPTION: non-maskable interupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Calls on our IDT
 */ 
int test_NMI_interupt(){
	// Use #2 for NM interupt
	TEST_HEADER;
	asm volatile("int $2");
	return FAIL;
}

/*
 *   Breakpoint trap
 *   DESCRIPTION: for setting a breakpoint
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Calls on our IDT
 */ 
int test_breakpoint(){
	// Use #3 for breakpoint
	TEST_HEADER;
	asm volatile("int $3");
	return FAIL;
}

/*
 *   test_overflow_interrupt
 *   DESCRIPTION: attempts to cause an overflow interrupt otherwise the test fails
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL, or nothing if the interrupt is triggered
 *   SIDE EFFECTS: Causes a divide by 0 error
 */ 
int test_overflow_interrupt() {
	TEST_HEADER;
	asm volatile("int $4");
	return FAIL;
}

/*
 *   test_bound_range_exceeded
 *   DESCRIPTION: Attempt to access an array out of bounds
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Causes a bound range exceeded error
 */ 
int test_bound_range_exceeded() {
	TEST_HEADER;
	int a[2];
	a[3] = 10;
	return FAIL;
}

/*
 *   invalid_opcode
 *   DESCRIPTION: checks to see if it is an invalid opcode
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Calls on our IDT
 */ 
int test_invalid_opcode(){
	// Use #6 for invalid opcode
	TEST_HEADER;
	asm volatile("int $6");
	return FAIL;
}

/*
 *   device_not_available
 *   DESCRIPTION: Checks to see if device not available
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Calls on our IDT
 */ 
int test_device_not_available(){
	// Use #7 for reserved interupt
	TEST_HEADER;
	asm volatile("int $7");
	return FAIL;
}

/*
 *   test_double_fault
 *   DESCRIPTION: check for double fault
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Calls on IDT
 */ 
int test_double_fault(){
	TEST_HEADER;
	asm volatile("int $8");
	return FAIL;
}

/*
 *   segment_overrun
 *   DESCRIPTION: floating point instruction
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Calls IDT
 */ 
int test_segment_overrun(){
	TEST_HEADER;
	asm volatile("int $9");
	return FAIL;
}

/*
 *   Invalid_tss
 *   DESCRIPTION: task switch
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Calls IDT
 */ 
int test_invalid_tss(){
	TEST_HEADER;
	asm volatile("int $10");
	return FAIL;
}

/*
 *   segment_not_present
 *   DESCRIPTION: interupts if segment not present
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Calls IDT
 */ 
int test_segment_not_present(){
	TEST_HEADER;
	asm volatile("int $11");
	return FAIL;
}

/*
 *   test stack fault
 *   DESCRIPTION: testing to see if fault stacks
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Calls IDT
 */ 
int test_stack_fault(){
	TEST_HEADER;
	asm volatile("int $12");
	return FAIL;
}

/*
 *   test_general_protection
 *   DESCRIPTION: force a general protection interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a general protection interrupt
 */ 
int test_general_protection(){
	TEST_HEADER;
	asm volatile("int $13");
	return FAIL;
}

/*
 *   test_page_fault
 *   DESCRIPTION: force page fault interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a page fault interrupt
 */ 
int test_page_fault(){
	TEST_HEADER;
	asm volatile("int $14");
	return FAIL;
}

/*
 *   test_x86_FPU_error
 *   DESCRIPTION: force x86 FPU error check interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a x86 FPU error interrupt
 */ 
int test_x86_FPU_error(){
	TEST_HEADER;
	asm volatile("int $16");
	return FAIL;
}

/*
 *   test_alignment_check
 *   DESCRIPTION: force interrupt alignment check interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a alignment check interrupt
 */ 
int test_alignment_check(){
	TEST_HEADER;
	asm volatile("int $17");
	return FAIL;
}

/*
 *   test machine check
 *   DESCRIPTION: force interrupt machine check
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a machine check interrupt
 */ 
int test_machine_check(){
	TEST_HEADER;
	asm volatile("int $18");
	return FAIL;
}

/*
 *   test_simd_floating_point_exception
 *   DESCRIPTION: force interrupt SIMD floating-point interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a simd floating point interrupt
 */ 
int test_simd_floating_point_exception(){
	TEST_HEADER;
	asm volatile("int $19");
	return FAIL;
}

/*
 *   test_syscall_handler
 *   DESCRIPTION: Call a system call to see if it triggers interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes a system call interrupt
 */ 
int test_syscall_handler() {
	TEST_HEADER;
	asm volatile("int $0x80");
	return FAIL;
}

/*
 *   test_paging
 *   DESCRIPTION: Testing to see if we can access various points in memory
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: PASS if we can access all points in memory
 *   SIDE EFFECTS: may cause page fault if initialization failed
 */
int test_paging() {
	TEST_HEADER;
	uint8_t a;
	uint8_t* p;

	p = (uint8_t *) 0x400000; // point to beginning of kernel memory
    printf("Point to beginning of kernel memory: 0x%x.\n", p);
    a = *p;
    printf("PASSED\n");

    p = (uint8_t *) 0x600000; // point to beginning of kernel memory
	printf("Point to middle of kernel memory: 0x%x.\n", p);
    a = *p;
    printf("PASSED\n");

    p = (uint8_t *) 0x7FFFFF; // point to end of kernel memory
    printf("Pointing to end of kernel memory: 0x%x.\n", p);
    a = *p;
    printf("PASSED\n");

    p = (uint8_t *) 0xB8000; // point to beginning of video memory
    printf("Pointing to beginning of video memory: 0x%x.\n", p);
    a = *p;
    printf("PASSED\n");

    p = (uint8_t *) 0xB8FFF; // point to end of vide memory
    printf("Pointing to end of video memory: 0x%x.\n", p);
    a = *p;
    printf("PASSED\n");

	return PASS;
}

/*
 *   test_page_fault_handler
 *   DESCRIPTION: Testing to see if we page fault when we dereference NULL
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Causes page fault interrupt
 */
int test_page_fault_handler() {
	TEST_HEADER;
	uint8_t a;
	uint8_t* p;

    p = (uint8_t *) 0x400000; // point to kernel memory
    printf("Point to beginning of kernel memory: 0x%x.\n", p);
    a = *p;
    printf("PASSED\n");

	p = (uint8_t *) 0x2;
    printf("Pointing to not present memory: 0x%x.\n", p);
    a = *p;
    printf("FAILED\n");

	return FAIL;
}

/*
 *   test_null
 *   DESCRIPTION: Testing to see if we page fault when we dereference NULL
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: FAIL if it doesn't trigger interrupt
 *   SIDE EFFECTS: Will print out error message and interrupt should occur
 */
int test_null() {
	TEST_HEADER;
	uint8_t a;
    uint8_t* p;

    p = (uint8_t *) 0;
	printf("Pointing to NULL: 0x%x.\n", p);
	a = *p;

	return FAIL;
}

/* Checkpoint 2 tests */

int test_rtc_driver() {
	TEST_HEADER;
	int print_repeat = 10;
	int print_counter = 0;
	int wait_ct = 0; 
	int print_char_counter = 0;
	int freq;
	unsigned char blank_buf;

	rtc_open(0);
	
	for (freq = 4; freq < 1025; freq *= 2) {
		rtc_write(0, &freq, 1);
		for(print_counter = 0; print_counter<print_repeat; print_counter++) {
			
			for (wait_ct = 0; wait_ct<freq; wait_ct+=4) {
				rtc_read(0, &blank_buf, 1);
			}
	
			for (print_char_counter = 0; print_char_counter<wait_ct; print_char_counter++) {
				printf("1");
			}		
		}
		
		clear();
	}

	return PASS;
}

int test_terminal_driver() {
	char buf[128];
	int num_char;
	while(1){
		num_char = terminal_read(0, &buf, 128);
		printf("You typed %d chars: ", num_char);
		terminal_write(0, &buf, num_char);
	}
};

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/*
 *   launch_tests
 *   DESCRIPTION: begin of tests
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: performs test indicated by TEST_VECTOR
 */ 
void launch_tests() {
	clear();

	TEST_OUTPUT("Testing RTC Driver", test_rtc_driver());
	// TEST_OUTPUT("Testing Terminal Driver", test_terminal_driver());



	// TEST_OUTPUT("General IDT Test", idt_test());
	// TEST_OUTPUT("Divide Error Test", test_divide_error());
	// TEST_OUTPUT("Bound Range Exceeded Test", test_bound_range_exceeded());
	// TEST_OUTPUT("System Call Test", test_syscall_handler());
	// TODO: add RTC test call here
	// TEST_OUTPUT("Paging Test", test_paging());
	// TEST_OUTPUT("Page Fault Test", test_page_fault_handler());
	// TEST_OUTPUT("NULL Dereference Test", test_null());
	// TEST_OUTPUT("NMI interrupt test", test_NMI_interupt());
	// TEST_OUTPUT("Breakpoint test", test_breakpoint());
	// TEST_OUTPUT("Overflow interrupt test", test_overflow_interrupt());
	// TEST_OUTPUT("Invalid opcode test", test_invalid_opcode());
	// TEST_OUTPUT("Device not available test", test_device_not_available());
	// TEST_OUTPUT("Double fault test", test_double_fault());
	// TEST_OUTPUT("Segment overrun test", test_segment_overrun());
	// TEST_OUTPUT("Invalid tss test", test_invalid_tss());
	// TEST_OUTPUT("Segment not present test", test_segment_not_present());
	// TEST_OUTPUT("Stack fault test", test_stack_fault());
	// TEST_OUTPUT("General protection test", test_general_protection());
	// TEST_OUTPUT("Page fault test", test_page_fault());
	// TEST_OUTPUT("Implemented paging test", test_paging());
	// TEST_OUTPUT("x86_FPU error test", test_x86_FPU_error());
	// TEST_OUTPUT("Alignment check test", test_alignment_check());
	// TEST_OUTPUT("Machine check test", test_machine_check());
	// TEST_OUTPUT("Simd floating point exception test", test_simd_floating_point_exception());

	// To test keyboard, set RUN_TESTS to 0 or comment all tests above
}
