
/*
The RTC once enabled will simply generate a clock cycle periodically at IRQ8.
Therefore, I THINK all we need to do is enable as well as POSSIBLY
setting the frequency to the value we want it set as.

IMPORTANT: What is important is that if register C is not read after an IRQ 8, then the interrupt will not happen again!!!

*/

#include "rtc.h"
#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"
#include "../syscall_helpers.h"


volatile int clock_count[3];
static int wait_count[3];

/*
 *   init_rtc
 *   DESCRIPTION: Intializes the RTC for interrupt generation
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Sets bit 6 of register B to 1 (this allows for timer interrupts)
 *                 Sets the rate valid to register A to change speed of RTC (if needed)
 */ 
void init_rtc() {
    // Register B's 6th bit is the enable bit for the timer intertupt
    // Therefore, we need to feed in the prev value as well as setting the 6th bit    
    outb(0x8B, RTC_PORT_COMMAND);		// select register B, and disable NMI
    char prev = inb(RTC_PORT_DATA);	    // read the current value of register B

    outb(0x8B, RTC_PORT_COMMAND);		// set the index again (a read will reset the index to register D)
    outb(prev | 0x40, RTC_PORT_DATA);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    // For testing purposes, we want to change the rate of RTC so
    // it doesn't spam the screen when called in our handler
    outb(0x8A, RTC_PORT_COMMAND);		// reset index to A
    prev = inb(RTC_PORT_DATA);

    // Standard rate value is 6, it can be between 2 to 15 ranging from 2Hz to 32,xxx Hz
    // Higher rate means slower clock, the rate is fed into bits 0-3 in register A
    int rate = 0x06; // set to max freq to virtualize
    outb(0x0A, RTC_PORT_COMMAND); // Might need to make this 8A but we want to reenable NMIs
    outb((prev & 0xF0) | rate, RTC_PORT_DATA); //write only our rate to A. Note, rate is the bottom 4 bits.

    // Enable both the primary PIC IRQ2 port as well as IRQ0 on the secondary PIC
    enable_irq(2);
    enable_irq(8);

    clock_count[0] = 0;
    clock_count[1] = 0;
    clock_count[2] = 0;
    wait_count[0] = 0;
    wait_count[1] = 0;
    wait_count[2] = 0;
}

/*
 *   rtc_handler
 *   DESCRIPTION: The handler for when the RTC timer interrupt occurs
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Clears out status register C so we can receive another timer interrupt
 */ 
void rtc_handler() {
    //printf("%d\n", clock_count);
    rtc_int_flag = 1;
    clock_count[0]++;
    clock_count[1]++;
    clock_count[2]++;
    
    // if (clock_count == freq)
    // {
    //     clock_count = 0;
    //     // update_attrib();
    //     // test_interrupts();
    // }
    
    // Register C let's us know which interrupt flag was set (there are more types of interrupt for RTC outside of timer)
    // If you do not clear these flags, then RTC will no longer trigger interrupts
    // Luckily for us, when you read from register C, it clears the flag contents
    // Therefore, we have to simply read from register C and just throwaway the date since we don't need it
    outb(0x0C, RTC_PORT_COMMAND);	// select register C
    inb(RTC_PORT_DATA);		        // just throw away contents
    
    rtc_int_flag = 0;
    send_eoi(8);
}

/* rtc_open
 * Inputs: filename
 * Return Value: 0
 * Function: sets starting settings */
int32_t rtc_open(const uint8_t * filename) {
    cli();
    wait_count[get_schedule_idx()] = RTC_MAX_FREQ / RTC_INIT_FREQ;
    clock_count[get_schedule_idx()] = 0;
    sti();
    // copy stuff and set up dentry for rtc.
    return 0;
}

/* 
 * rtc_close
 *   DESCRIPTION: closes the rtc
 *   INPUTS: fd - file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failure
 *   SIDE EFFECTS: modifies the file descriptor array
 */
int32_t rtc_close(int32_t fd) {
    cli();
    pcb_t * pcb = get_curr_pcb_ptr();
    pcb->file_desc_arr[fd].flags = 0;
    wait_count[get_schedule_idx()] = 0;
    clock_count[get_schedule_idx()] = 0;
    sti();
    return 0;
}

/* 
 * rtc_read
 * Inputs: fd, buf, nbytes
 * Return Value: 0, always suceeds
 * Function: holds and returns when an RTC interupt occurs */
int32_t rtc_read(int32_t fd, void * buf, int32_t nbytes) {
    sti();
    while (clock_count[get_schedule_idx()] <= wait_count[get_schedule_idx()]); // wait to get response
    cli();
    clock_count[get_schedule_idx()] = 0; //reset
    sti();
    while (rtc_int_flag != 0); //wait until not interupting to return
    
    return 0;
}

/* rtc_write
 * Inputs: fd, buf, nbytes
 * Return Value: 0 for sucess -1 for fail
 * Function: writes the input frequency from buf, to set the frequency of RTC interupts */
int32_t rtc_write(int32_t fd, const void * buf, int32_t nbytes) {
    cli();
    int freq = *(const int *)buf; //load freq
    // freq *= 8;
    if (freq < 2 || freq > 1024) return -1; // param check
    if (freq && (! (freq & (freq-1)))) { //check power of 2
        wait_count[get_schedule_idx()] = RTC_MAX_FREQ/freq; //update settings
        clock_count[get_schedule_idx()] = 0;
        return 0;
    }
    sti();
    return -1;
};

// USEFUL WEBSITE FOR UDERSTANDING THE REGISTER CONTENTS FOR RTC
// https://web.archive.org/web/20150514082645/http://www.nondot.org/sabre/os/files/MiscHW/RealtimeClockFAQ.txt

// USEFUL WEBSITE FOR ENABLING RTC NAD CHANGING REGISTER CONTENTS
// https://wiki.osdev.org/RTC
