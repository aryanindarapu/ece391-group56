
/*
The RTC once enabled will simply generate a clock cycle periodically at IRQ8.
Therefore, I THINK all we need to do is enable as well as POSSIBLY
setting the frequency to the value we want it set as.

IMPORTANT: What is important is that if register C is not read after an IRQ 8, then the interrupt will not happen again!!!

*/

#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"
#include "rtc.h"

static int case_count;
static int clock_count;

static int rtc_interupt_flag;

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
    int rate = 0x0F;
    outb(0x0A, RTC_PORT_COMMAND); // Might need to make this 8A but we want to reenable NMIs
    outb((prev & 0xF0) | rate, RTC_PORT_DATA); //write only our rate to A. Note, rate is the bottom 4 bits.

    // Enable both the primary PIC IRQ2 port as well as IRQ0 on the secondary PIC
    enable_irq(2);
    enable_irq(8);

    case_count = 0;
    clock_count = 0;
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
    // Clear interupt flag for the rtc_read
    rtc_interupt_flag = 0;

    // printf("RTC Time interrupt!\n");
    clock_count++;
    if(clock_count == 100)
    {
        clock_count = 0;
        // update_attrib();
        // test_interrupts();
        //clear();
        //case_count += 1;
        //printf(" TIME: %d", case_count);
    }
    
    // Register C let's us know which interrupt flag was set (there are more types of interrupt for RTC outside of timer)
    // If you do not clear these flags, then RTC will no longer trigger interrupts
    // Luckily for us, when you read from register C, it clears the flag contents
    // Therefore, we have to simply read from register C and just throwaway the date since we don't need it
    outb(0x0C, RTC_PORT_COMMAND);	// select register C
    inb(RTC_PORT_DATA);		        // just throw away contents

    send_eoi(8);
}

int32_t rtc_read(int32_t fd, void * buf, int32_t nbytes) {
    rtc_interupt_flag = 1;

    // MP3.2 TODO: Is this fine since RTC is hardware?
    while(rtc_interupt_flag) {};

    // Always return 0 for RTC
    return 0;
}

int32_t rtc_write(int32_t fd, const void * buf, int32_t nbytes) {
    uint8_t * hz_rate = (uint8_t *) buf;
    int count = 0;
    char prev;

    // Get the 32-bit value Hz rate
    uint32_t rate = hz_rate[0] | (hz_rate[1] << 8) | (hz_rate[2] << 16) | (hz_rate[3] << 24);

    // Check to make sure Hz rate is less than or equal 8192 Hz
    if(rate > 8192 || (rate - 1) & rate != 0) {
        return -1;
    }

    // MP3.2 TODO: CHECK IF WE NEED TO ACCOUNT FOR 0HZ TO DISABLE RTC
    //              ALSO CHECK FOR CASE OF 1HZ

    // Make sure that Hz rate is a power of 2
    while(rate != 1) {
        count++;
        rate /= 2;
    }

    // Rate is calculated by 32768 >> (rate - 1) in our status register
    // That's why we keep a count variable and shift depending on how many powers of 2 we have
    rate = 16 - count;

    outb(0x8A, RTC_PORT_COMMAND); // Grab register A contents
    prev = inb(RTC_PORT_DATA);    // Store in prev
    
    outb(0x0A, RTC_PORT_COMMAND); // Reenable NMI
    // Rate will be between 3 and 15
    outb((prev & 0xF0) | rate, RTC_PORT_DATA); //write our rate to A. Note, rate is the bottom 4 bits.

    // Always return 0
    return 0;
}

int32_t rtc_open(const uint8_t * filename) {
    // RTC_OPEN SHOULD RESET FREQUENCY TO 2HZ
    char prev;

    outb(0x8A, RTC_PORT_COMMAND);	// reset index to A
    prev = inb(RTC_PORT_DATA);      // grab previous value constraints

    // Standard rate value is 6, it can be between 2 to 15 ranging from 2Hz to 32,xxx Hz
    // Higher rate means slower clock, the rate is fed into bits 0-3 in register A
    // Rate of 15 sets frequency to 2Hz
    int rate = 0x0F;
    outb(0x0A, RTC_PORT_COMMAND); // Might need to make this 8A but we want to reenable NMIs
    outb((prev & 0xF0) | rate, RTC_PORT_DATA); //write only our rate to A. Note, rate is the bottom 4 bits.

    // MP3.2 TODO: Does this return 0 or a file descriptor?
    return 0;
}

int32_t rtc_close(int32_t fd) {
    // MP3.2 TODO: Do we actually need to close RTC?
    return -1;
}


// USEFUL WEBSITE FOR UDERSTANDING THE REGISTER CONTENTS FOR RTC
// https://web.archive.org/web/20150514082645/http://www.nondot.org/sabre/os/files/MiscHW/RealtimeClockFAQ.txt

// USEFUL WEBSITE FOR ENABLING RTC NAD CHANGING REGISTER CONTENTS
// https://wiki.osdev.org/RTC
