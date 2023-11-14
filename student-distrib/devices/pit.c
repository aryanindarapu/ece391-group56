#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"
#include "rtc.h"
#include "../syscall_helpers.h"
#include "../process.h"
#include "pit.h"

int32_t schedule_index = 0;
/*
 *  0x40    Channel 0 data port (read/write) for the PIT
 *
 *  MAY NEED TO CHANGE COMMAND REGISTER (0x43) TO SELECT CHANNEL 0
 *  
 *  https://www.cs.princeton.edu/courses/archive/fall18/cos318/precepts/3-preemptive.pdf#:~:text=Preemptive%20Scheduling%20Tasks%20are%20preempted%20via%20timer%20interrupt,Save%20the%20current%20task%20%28context%20switch%29%20before%20preempting
 *
 *
 *
 */


void init_pit() {
    // Disable interrupts
    cli();

    // Select channel 0 in the mode/command register (port 0x43)
    // This also sends out the interrupt signal to PIC when count reaches 0
    // Resetting the counter and sending the signal is handled for us
    outb(0x00, PIT_COMMAND);

    // Need to set the rate for the pit (2^17 ~= 100ms between each IRQ)
    int count = (65536 * 2) - 1;

    outb(count & 0xFF, PIT_CHANNEL0_DATA);
    outb((count & 0xFF00) >> 8, PIT_CHANNEL0_DATA);

    enable_irq(0);

    sti();
}

int pit_handler() {
    cli();
    process_switch(schedule_index);
    schedule_index = (schedule_index + 1) % 3;
    sti();
    send_eoi(0);
    return 0;
    //process_switch();
}


