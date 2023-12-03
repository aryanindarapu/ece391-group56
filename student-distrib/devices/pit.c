#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"
#include "rtc.h"
#include "../syscall_helpers.h"
#include "../process.h"
#include "pit.h"
#include "../terminal.h"

int32_t schedule_index = 0;
int32_t init_schedule_index = 0;
extern int new_terminal_flag;
extern int temp_terminal_flag;
int terminals_initialized = 0;
int init_wait_count = 50;
int pit_wait_count = 0;

int is_terminals_initialized()
{
    return terminals_initialized;
}
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
    
    // Select channel 0 in the mode/command register (port 0x43)
    // This also sends out the interrupt signal to PIC when count reaches 0
    // Resetting the counter and sending the signal is handled for us
    outb(0x36, PIT_COMMAND);

    // Need to set the rate for the pit (2^17 ~= 100ms between each IRQ)
    int count = 30000;

    outb(count & 0xFF, PIT_CHANNEL0_DATA);
    outb(count >> 8, PIT_CHANNEL0_DATA);


    enable_irq(0);
}

int get_schedule_idx()
{
    if(!terminals_initialized)
        return init_schedule_index;
    return schedule_index;
}

void set_schedule_idx(int index)
{
    schedule_index = index;
}

int pit_handler () {

    if(!terminals_initialized)
    {
        if(init_schedule_index < 3 && pit_wait_count == init_wait_count)
        {
            new_terminal_flag = 1;
            init_schedule_index ++;
            pit_wait_count = 0;
            asm volatile (
                "movl %%esp, %0   ;\
                movl %%ebp, %1   ;\
                "
                : "=r" (get_curr_pcb_ptr()->kernel_esp), "=r" (get_curr_pcb_ptr()->kernel_ebp)
                :
                : "memory"
            );
            execute((const uint8_t *) "shell");
        }
        else if(init_schedule_index == 3  && pit_wait_count == init_wait_count)
        {
            terminal_switch(0);
            terminals_initialized = 1;
            schedule_index = 2;
        }
        else
        {
            pit_wait_count++;
            send_eoi(0);
            return 0;
        }
    }
    
    asm volatile (
        "movl %%esp, %0   ;\
         movl %%ebp, %1   ;\
        "
        : "=r" (get_curr_pcb_ptr()->kernel_esp), "=r" (get_curr_pcb_ptr()->kernel_ebp)
        :
        : "memory"
    );

    schedule_index++;
    schedule_index %= 3;

    pcb_t * next_pcb = get_child_pcb(schedule_index); // bad
    tss.ss0 = (uint16_t) KERNEL_DS;
    tss.esp0 = (uint32_t) next_pcb + EIGHT_KB - STACK_FENCE_SIZE;
    
    setup_user_page(((next_pcb->pid * FOUR_MB) + EIGHT_MB) / FOUR_KB);
    send_eoi(0);
    
    asm volatile (
        "movl %%eax, %%esp   ;\
         movl %%ebx, %%ebp   ;\
        "
        :
        : "a" (next_pcb->kernel_esp), "b" (next_pcb->kernel_ebp)
        : "memory"
    );
    
    return 0;

}




