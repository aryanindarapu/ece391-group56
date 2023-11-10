#include "process.h"
#include "syscall.h"
#include "syscall_helpers.h"

int32_t active_terminal_index; // TODO: this is changed by the keyboard driver for terminal switching
int32_t schedule_index;

void init_shells() {
    int i;
    for (i = 0; i < 3; i++) {
        execute("shell");
        terminal_indices[i] = get_pcb_ptr(i); // puts pointer of current pcb into terminal struct
    }

    schedule_index = 0;
}

int32_t process_switch() {
    // Get to end of linked list
    pcb_t * curr_pcb = get_pcb_ptr(schedule_index);

    while (curr_pcb->child_pid != -1) {
        curr_pcb = get_pcb_ptr(curr_pcb->child_pid);
    }

    // At this point, we have the child process that needs to be executed and need to execute context switch
    // Also need to change vidmap page mapping

    if (schedule_index == active_terminal_index) {
        // We are writing to active terminal page
    } else {
        // write to non-display memory
    }

    // Change to use different vidmap page mapping
    schedule_index++;
    if (schedule_index >= 3) schedule_index = 0;

    return 0;
}