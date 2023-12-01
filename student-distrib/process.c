#include "process.h"
#include "syscall.h"
#include "terminal.h"
#include "syscall_helpers.h"
#include "x86_desc.h"

// int32_t active_terminal_index; // TODO: this is changed by the keyboard driver for terminal switching

// void init_shells() {
//     int i;
//     for (i = 0; i < 3; i++) {
//         execute("shell");
//         terminal_indices[i] = get_pcb_ptr(i); // puts pointer of current pcb into terminal struct
//     }
//     schedule_index = 0;
// }

int32_t process_switch(int32_t terminal_num) {
    // Get to end of linked list
    pcb_t * curr_pcb = get_child_pcb(terminal_num); 

    // At this point, we have the child process that needs to be executed and need to execute context switch
    // Also need to change vidmap page mapping

    // if (schedule_index == active_terminal_index) {
    //     // We are writing to active terminal page
    // } else {
    //     // write to non-display memory
    // }
    
    setup_user_page(((curr_pcb->pid * FOUR_MB) + EIGHT_MB) / FOUR_KB);
    flush_tlb();

    tss.ss0 = KERNEL_DS;
    tss.esp0 = (uint32_t) curr_pcb + EIGHT_KB - STACK_FENCE_SIZE;
    

    // TODO: set up TSS
    // store kernel esp and ebp in the pcb
    // asm volatile (
    //     "movl %%esp, %%eax   ;\
    //      movl %%ebp, %%ebx   ;\
    //     "
    //     : "=a" (curr_pcb->kernel_esp), "=b" (curr_pcb->kernel_ebp)
    //     :
    //     : "memory"
    // );

    // int32_t output;
    
    /* enable interrupts*/
    // sets up DS, ESP, EFLAGS, CS, EIP onto stack for context switch
    // asm volatile ("\
    //     andl $0x00FF, %%eax     ;\
    //     movw %%ax, %%ds         ;\
    //     pushl %%eax             ;\
    //     pushl %%ebx             ;\
    //     pushfl                  ;\
    //     pushl %%ecx             ;\
    //     pushl %%edx             ;\
    //     iret                    ;\
    //     "
    //     : "=a" (output)
    //     : "a" (USER_DS), "b" (curr_pcb->user_esp), "c" (USER_CS), "d" (curr_pcb->user_eip) 
    //     : "memory"
    // );

    // NOTE: will it return back here? This context switch probably isn't exactly correct
    return 0;
}

int32_t process_vidmap_change(int32_t terminal_from, int32_t terminal_to) {
    return 0;    
}

pcb_t * get_child_pcb(int32_t terminal_num) 
{
    pcb_t * curr_pcb = get_pcb_ptr(get_terminal_arr(terminal_num));
    
    while (curr_pcb->child_pid != -1) {
        curr_pcb = get_pcb_ptr(curr_pcb->child_pid);
    }

    return curr_pcb;
}

int is_pcb_available()
{
    int i;
    for(i =0; i<MAX_NUM_PROGRAMS; i++)
    {
        if(pcb_flags[i] == 0)
        {
            return 1;
        }
    }
    return 0;
}