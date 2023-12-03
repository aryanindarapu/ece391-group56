#include "i8259.h"
#include "../lib.h"
#include "../x86_desc.h"
#include "rtc.h"
#include "../syscall_helpers.h"
#include "pit.h"
#include "../terminal.h"

int32_t schedule_index = 0;
int32_t init_schedule_index = 0;
extern int new_terminal_flag;
int terminals_initialized = 0;
int init_wait_count = 50;
int pit_wait_count = 0;

/*
 *   is_terminals_initialized
 *   DESCRIPTION: Check to see if our terminals are finished initializing
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */  
int is_terminals_initialized()
{
    return terminals_initialized;
}

/*
 *   init_pit
 *   DESCRIPTION: initializes the PIT by setting the frequency and enabling irq
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enables PIT for round robin scheduling
 */  
void init_pit() {
    
    // Select channel 0 in the mode/command register (port 0x43)
    // This also sends out the interrupt signal to PIC when count reaches 0
    // Resetting the counter and sending the signal is handled for us
    outb(0x36, PIT_COMMAND);

    // Need to set the rate for the pit (2^17 ~= 100ms between each IRQ)
    // 30,000 should be around ~25ms
    int count = 30000;

    // Enter it into the channel to set the PIT frequency
    outb(count & 0xFF, PIT_CHANNEL0_DATA);
    outb(count >> 8, PIT_CHANNEL0_DATA);

    // Enable the irq on the PIC
    enable_irq(0);
}

/*
 *   get_schedule_idx
 *   DESCRIPTION: Grabs the index of the current scheduled process
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */  
int get_schedule_idx()
{
    // Conditional check to see if we need to initialize our terminals
    if(!terminals_initialized)
        return init_schedule_index;
    return schedule_index;
}

/*
 *   set_schedule_idx
 *   DESCRIPTION: Updates the schedule index
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */  
void set_schedule_idx(int index)
{
    schedule_index = index;
}

/*
 *   pit_handler
 *   DESCRIPTION: Saves previous state of old process and sets up for the next scheduled process
 *   INPUTS: none
 *   OUTPUTS: int - doesn't do anything though
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Changes vid mapping to current scheduled terminal as well as saving stack and base pointers for old process
 */  
void pit_handler () {
    // If terminals are not initialized, there is additional work we need to do
    if (!terminals_initialized)
    {
        // Check to see if we still need to execute the additional terminals
        if (init_schedule_index < 3)
        {
            // Set new terminal flag and increment the schedule index to setup new terminal
            new_terminal_flag = 1;
            init_schedule_index ++;
            
            // Save stack pointer for iret context in current pcb struct
            asm volatile (
                "movl %%esp, %0   ;\
                movl %%ebp, %1   ;\
                "
                : "=r" (get_curr_pcb_ptr()->kernel_esp), "=r" (get_curr_pcb_ptr()->kernel_ebp)
                :
                : "memory"
            );

            // Boot up new terminal
            execute((const uint8_t *) "shell");
        }
        // Finished setting up all terminals
        else if (init_schedule_index == 3)
        {
            // Switch back to first process and let program know we are done with setup
            terminal_switch(0);
            terminals_initialized = 1;
            schedule_index = 2;
        }
    }

    // Increment scheduling index to next terminal
    schedule_index++;
    schedule_index %= 3;

    // Save stack and base pointer for iret context in current pcb struct
    asm volatile (
        "movl %%esp, %0   ;\
         movl %%ebp, %1   ;\
        "
        : "=r" (get_curr_pcb_ptr()->kernel_esp), "=r" (get_curr_pcb_ptr()->kernel_ebp)
        :
        : "memory"
    );

    // Grab the next child pcb for the next scheduled item
    pcb_t * next_pcb = get_child_pcb(schedule_index);

    // Update task segment selector
    tss.ss0 = (uint16_t) KERNEL_DS;
    tss.esp0 = (uint32_t) next_pcb + EIGHT_KB - STACK_FENCE_SIZE;
    
    // Swap vid map for the user page
    setup_user_page(((next_pcb->pid * FOUR_MB) + EIGHT_MB) / FOUR_KB);
    
    // Grab the esp and ebp to jump back to the current scheduled process
    asm volatile (
        "movl %%eax, %%esp   ;\
         movl %%ebx, %%ebp   ;\
        "
        :
        : "a" (next_pcb->kernel_esp), "b" (next_pcb->kernel_ebp)
        : "memory"
    );
    send_eoi(0);    
}
