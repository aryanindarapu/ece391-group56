#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"
#include "rtc.h"
#include "../syscall_helpers.h"
#include "../process.h"
#include "pit.h"
#include "../terminal.h"

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
    outb(0x36, PIT_COMMAND);

    // Need to set the rate for the pit (2^17 ~= 100ms between each IRQ)
    int count = 30000;

    outb(count & 0xFF, PIT_CHANNEL0_DATA);
    outb(count >> 8, PIT_CHANNEL0_DATA);


    enable_irq(0);
}

int pit_handler(){//int eip, int esp) {
    uint32_t a;
    uint32_t b;
    asm volatile ("\
        "
        : "=a" (a), "=b" (b)
        :
        : "memory"
    );
    // printf("%d %d \n", a , b);
    send_eoi(0);
    if(0==0)//is_started()==0)
    {
        return 0;
    } 

    // asm volatile ("\
    //     addl $3, %%esp          ;\
    //     popfl                   ;\
    //     popal                   ;\
    //     iret                    ;\
    //     "
    //     : 
    //     :
    //     : "memory"
    // );
    
    /* grab esp eip, store in pcb, replace with next pcb eip, esp, use iret*/
    pcb_t * curr_pcb = get_child_pcb(schedule_index);
    
    curr_pcb->user_eip = a;
    curr_pcb->user_esp = b;
    
    do
    {
        schedule_index ++;
        schedule_index %= 3;
    } while(get_terminal_arr(schedule_index)!=-1);
        
    pcb_t * next_pcb = get_child_pcb(get_terminal_arr(schedule_index));
    
    tss.ss0 = (uint16_t) KERNEL_DS;
    tss.esp0 = (uint32_t) next_pcb + EIGHT_KB - STACK_FENCE_SIZE; // offset of kernel stack segment
    
    asm volatile ("\
        andl $0x00FF, %%eax     ;\
        movw %%ax, %%ds         ;\
        pushl %%eax             ;\
        pushl %%ebx             ;\
        pushfl                  ;\
        pushl %%ecx             ;\
        pushl %%edx             ;\
        iret                    ;\
        "
        : 
        : "a" (USER_DS), "b" (next_pcb->user_esp), "c" (USER_CS), "d" (next_pcb->user_eip) 
        : "memory"
    );
    // asm volatile ("\
    //     "
    //     : "=a" (curr_pcb->user_eip), "=b" (curr_pcb->user_esp)
    //     :
    //     : "memory"
    // );

    

    // asm volatile ("\
    //     movl %%eax, 8(%%esp)     ;\
    //     movl %%ebx, 20(%%esp)   ;\
    //     movl $8, %%esp          ;\
    //     iret                    ;\
    //     "
    //     :
    //     : "a" (next_pcb->user_eip), "b" (next_pcb->user_esp)
    //     : "memory"
    // );

    return 0;
    // update pcb for current process's eip esp
    // jump with process_switch()
    // set up iret context for rest
    // send_eoi(0);
    // return 0;
    // cli();
    // process_switch(schedule_index);
    // schedule_index = (schedule_index + 1) % 3;
    // sti();
    // send_eoi(0);
    // return 0;
    //process_switch();
}


