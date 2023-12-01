#include "syscall.h"
#include "file_system_driver.h"
#include "syscall_helpers.h"
#include "paging.h"
#include "lib.h"
#include "x86_desc.h"
#include "exceptions.h"
#include "process.h"
#include "terminal.h"

extern int terminal_idx;
extern int new_terminal_flag;

/* 
 * execute
 *   DESCRIPTION: Executes a program with the given arguments. 
 *   INPUTS: command - the command to execute
 *   OUTPUTS: none
 *   RETURN VALUE: 0 to 255 on sucess, 256 on die by exception, -1 on failure
 *   SIDE EFFECTS: sets up user page, changes tss, and initializes a new pcb
 */
int32_t execute (const uint8_t* command) {
    if(!is_pcb_available()) return -1;
    cli();
    if (command == NULL) return -1;
    int i;
    
    uint8_t filename[128];
    for (i = 0; i < 128; i++) {
        if (command[i] == ' ' || command[i] == '\0') {
            filename[i] = '\0';
            break;
        }

        filename[i] = command[i];
    }

    i++;
    
    /* Check if file is valid */
    dentry_t exec_dentry;
    if (read_dentry_by_name((const uint8_t *) filename, &exec_dentry) == -1) {
        return -1;
    }

    uint8_t magic_byte_buf[4];
    if (read_data(exec_dentry.inode_num, 0, magic_byte_buf, sizeof(uint32_t)) == -1) {
        return -1;
    }

    if (magic_byte_buf[0] != MAGIC_BYTE_0 || magic_byte_buf[1] != MAGIC_BYTE_1 || magic_byte_buf[2] != MAGIC_BYTE_2 || magic_byte_buf[3] != MAGIC_BYTE_3) {
        return -1;
    }

    /* Check if PCBs are available */
    uint32_t new_pid_idx;
    for (new_pid_idx = 0; new_pid_idx < MAX_NUM_PROGRAMS; new_pid_idx++) {
        if (pcb_flags[new_pid_idx] == 0) {
            break;
        }
    }

    if (new_pid_idx >= MAX_NUM_PROGRAMS) {
        return -1;
    }

    /////////////// POINT OF NO RETURN ///////////////
    /* Set up PCB */
    pcb_flags[new_pid_idx] = 1; // enable PCB block
    pcb_t * new_pcb = (pcb_t *)(EIGHT_MB - (new_pid_idx + 1) * EIGHT_KB); // puts PCB pointer at bottom of kernel memory

    // Set commands
    int offset = i;
    for (; i < LINE_BUFFER_SIZE; i++) {
        new_pcb->commands[i - offset] = command[i];
    }

    new_pcb->pid = new_pid_idx;
    new_pcb->child_pid = -1;  

    /* Set up parent and child pointers accordingly */
    // TODO: change this when dynamically loading shells
    // if (new_pid_idx == 0) { // i.e. the PCB is for the initial shell
    //     new_pcb->parent_pid = -1;
    // } else {
    //     new_pcb->parent_pid = get_curr_pcb_ptr()->pid; // point to parent PCB pointer
    //     get_curr_pcb_ptr()->child_pid = new_pid_idx;
    // }


    new_pcb->file_desc_arr[0].ops_ptr = stdin_ops_table;
    new_pcb->file_desc_arr[0].inode = -1;
    new_pcb->file_desc_arr[0].flags = 1;
    new_pcb->file_desc_arr[0].file_pos = 0;

    new_pcb->file_desc_arr[1].ops_ptr = stdout_ops_table;
    new_pcb->file_desc_arr[1].inode = -1;
    new_pcb->file_desc_arr[1].flags = 1;
    new_pcb->file_desc_arr[1].file_pos = 0;

    // new_pcb->screen_flag_set = 0;

    /* Set up 4MB page for user program */
    setup_user_page(((new_pid_idx * FOUR_MB) + EIGHT_MB) / FOUR_KB);

    /* Copy to user memory */
    read_data(exec_dentry.inode_num, 0, (uint8_t *) PROGRAM_START, ((inode_t *) (inode_ptr + exec_dentry.inode_num))->length);
    
    /* Save regs needed for PCB */
    // Read bytes 24 - 27 to get eip
    uint8_t eip_ptr[sizeof(uint32_t)];
    read_data(exec_dentry.inode_num, EIP_START, eip_ptr, sizeof(uint32_t));

    /* Set up TSS */ // TSS - contains process state information of the parent task to restore it
    tss.ss0 = (uint16_t) KERNEL_DS;
    tss.esp0 = (uint32_t) new_pcb + EIGHT_KB - STACK_FENCE_SIZE; // offset of kernel stack segment
    
    uint32_t user_eip = *((uint32_t *) eip_ptr);
    uint32_t user_esp = PROGRAM_START - STACK_FENCE_SIZE;

    /* push the user eip and esp to the cur pcb so we can restore context */
    new_pcb->user_esp = user_esp;
    new_pcb->user_eip = user_eip;
    // new_pcb->kernel_esp = new_pcb + EIGHT_KB - STACK_FENCE_SIZE;
    // new_pcb->kernel_ebp = new_pcb + EIGHT_KB - STACK_FENCE_SIZE;

    // store kernel esp and ebp in the pcb
    asm volatile (
        "movl %%esp, %%eax   ;\
         movl %%ebp, %%ebx   ;\
        "
        : "=a" (new_pcb->base_esp), "=b" (new_pcb->base_ebp)
        :
        : "memory"
    );
    
    if (new_pid_idx == 0 || new_terminal_flag) {
        new_pcb->parent_pid = -1;
        set_terminal_arr(terminal_idx, new_pid_idx);
        new_terminal_flag = 0; // reset flag
    } else {
        new_pcb->parent_pid = get_curr_pcb_ptr()->pid; // point to parent PCB pointer
        get_curr_pcb_ptr()->child_pid = new_pid_idx;
    }

    int32_t output;
    /* enable interrupts*/
    sti();
    // stack swap
    // asm volatile (
    //     "movl %%eax, %%esp   ;\
    //      movl %%ebx, %%ebp   ;\
    //     "
    //     :
    //     : "a" (new_pcb->kernel_esp), "b" (new_pcb->kernel_ebp)
    //     : "memory"
    // );
    // sets up DS, ESP, EFLAGS, CS, EIP onto stack for context switch
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
        : "a" (USER_DS), "b" (user_esp), "c" (USER_CS), "d" (user_eip) 
        : "memory"
    );

    asm volatile ("\
        ret_from_halt:     ;\
        movl %%eax, %0     ;\
        "
        : "=r" (output)
        : 
        : "memory"
    );

    if (exception_raised_flag) {
        exception_raised_flag = 0;
        return EXCEPTION_OCCURRED_VAL; 
    }

    sti();
    return output;
}

/* 
 * halt
 *   DESCRIPTION: Halts the currently executing program
 *   INPUTS: status - the status to return to the parent
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: removes current pcb, restores parent paging, and jumps to parent stack
 */
int32_t halt (uint8_t status) {
    // When closing, do I need to check if current PCB has any child PCBs?
    cli();
    pcb_t * pcb = get_curr_pcb_ptr();
    /* push user context if its base shell since we have no processes left */
    // TODO: change this when dynamically loading shells
    // Check if the pid is in the current active terminals array
    if (pcb->pid == get_terminal_arr(0) || pcb->pid == get_terminal_arr(1) || pcb->pid == get_terminal_arr(2)) {
        // recover context from halt(esp, eip, USER_CS, USER_DS);
        // 0x00FF - clears the bottom 8 bytes of the return value
        // 0x0200 - turns on bit of EFLAGS
        // sti();
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
            : "a" (USER_DS), "b" (pcb->user_esp), "c" (USER_CS), "d" (pcb->user_eip) 
            : "memory"
        );
    } 

    /* get pcb from cur pcbs parent PID*/
    pcb_t * parent_pcb = get_pcb_ptr(pcb->parent_pid);

    parent_pcb->child_pid = -1; // removes the child process
    // clear old commands
    int i;
    for (i = 0; i < LINE_BUFFER_SIZE; i++) {
        pcb->commands[i] = '\0';
    }

    // release FD array for this pcb
    for (i = 0; i < MAX_FILE_DESC; i++) {
        if (pcb->file_desc_arr[i].flags) {
            pcb->file_desc_arr->ops_ptr.close(i);
        }

        pcb->file_desc_arr[i].flags = 0;
    }


    /* remove current pcb from present flags */
    pcb_flags[pcb->pid] = 0;
    pcb->pid = -1;
    pcb->parent_pid = -1;
    
    /* Set TSS again */
    tss.ss0 = (uint16_t) KERNEL_DS; // segment selector for kernel data segment
    tss.esp0 = (uint32_t) parent_pcb + EIGHT_KB - STACK_FENCE_SIZE;; 
    
    /* Restore parent paging and flush tlb to update paging structure */
    setup_user_page(((parent_pcb->pid  * FOUR_MB) + EIGHT_MB) / FOUR_KB);
    /* Save process context (ebp, esp) then return to execute the next process */
    asm volatile ("\
        movl %%ebx, %%ebp      ;\
        movl %%ecx, %%esp      ;\
        movl %%edx, %%eax      ;\
        jmp ret_from_halt      ;\
        "
        : 
        : "b" (pcb->base_ebp), "c" (pcb->base_esp), "d" (status)
    );

    // if we get control back then we return fail(we shouldn't ever get control back)
    return -1;
}

/* 
* open
*   DESCRIPTION: Opens the file with the given filename
*   INPUTS: filename - the name of the file to open
*   OUTPUTS: none
*   RETURN VALUE: file descriptor index or -1 on failure
*   SIDE EFFECTS: modifies file descriptor array in pcb
*/
int32_t open (const uint8_t* filename) {
    dentry_t file_dentry;
    uint32_t fd;
    
    // ensure the filename is valid
    if (filename == NULL || strlen((const int8_t *) filename) > 32) {
        return -1;
    }

    pcb_t * pcb = get_curr_pcb_ptr();
    // ensure the file desc has space AND find the index to emplace this file
    for (fd = 2; fd < MAX_FILE_DESC; fd++) {
        // is the index empty?
        if (pcb->file_desc_arr[fd].flags == 0) {
            break; //this fd is the one we will emplace the file to 
        }
    }

    // Ensure there was space left
    if (fd >= MAX_FILE_DESC) {
        return -1;
    }

    /* The filename exists, the file_descriptor has space, and the dentry has been populated
       by the filesystem function read_dentry_by_name */
    /* So set all of the fields */
    if (read_dentry_by_name (filename, &file_dentry) == -1) { 
        return -1; //file doesn't exist
    }

    int32_t status;
    switch (file_dentry.file_type) {
        case 0: // rtc driver
            status = rtc_open(filename);
            if (fd < 0) return -1;
            pcb->file_desc_arr[fd].ops_ptr = rtc_ops_table;
            break;
        case 1: // dir
            status = dir_open(filename);
            if (fd < 0) return -1;
            pcb->file_desc_arr[fd].ops_ptr = dir_ops_table;
            break;
        case 2: // file
            status = file_open(filename);
            if (fd < 0) return -1;
            pcb->file_desc_arr[fd].ops_ptr = file_ops_table;
            break;
        default:
            return -1;
    }

    pcb->file_desc_arr[fd].flags = 1;
    pcb->file_desc_arr[fd].inode = file_dentry.inode_num;
    pcb->file_desc_arr[fd].file_pos = 0;
    return fd;
}

/* 
* close
*   DESCRIPTION: Closes the file with the given file descriptor
*   INPUTS: fd - the file descriptor to close
*   OUTPUTS: none
*   RETURN VALUE: 0 on success, -1 on failure
*   SIDE EFFECTS: modifies file descriptor array in pcb
*/
int32_t close (uint32_t fd) {
    if (fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    pcb_t * pcb = get_curr_pcb_ptr();
    if (!pcb->file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return pcb->file_desc_arr[fd].ops_ptr.close(fd);
}

/* 
* read
*   DESCRIPTION: Reads nbytes from the file with the given file descriptor
*   INPUTS: fd - the file descriptor to read from
*           buf - the buffer to read into
*           nbytes - the number of bytes to read
*   OUTPUTS: none
*   RETURN VALUE: number of bytes read on success, -1 on failure
*   SIDE EFFECTS: none
*/
int32_t read (uint32_t fd, void* buf, uint32_t nbytes) {
    if (fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    pcb_t * pcb = get_curr_pcb_ptr();
    if (!pcb->file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    // file_desc_t file_desc = pcb->file_desc_arr[fd];
    // inode_t * curr_inode = inode_ptr + file_desc.inode;

    // if (file_desc.file_pos >= curr_inode->length) return 0;
    return pcb->file_desc_arr[fd].ops_ptr.read(fd, buf, nbytes);
}

/* 
* write
*   DESCRIPTION: Writes nbytes to the file with the given file descriptor
*   INPUTS: fd - the file descriptor to write to
*           buf - the buffer to write from
*           nbytes - the number of bytes to write
*   OUTPUTS: none
*   RETURN VALUE: number of bytes written on success, -1 on failure
*   SIDE EFFECTS: none
*/
int32_t write (uint32_t fd, const void* buf, uint32_t nbytes) {
    if (fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    pcb_t * pcb = get_curr_pcb_ptr();
    if (!pcb->file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return pcb->file_desc_arr[fd].ops_ptr.write(fd, buf, nbytes);
}

/*
* getargs
*   DESCRIPTION: Copies the arguments into a user-level buffer
*   INPUTS: buf - the buffer to copy the arguments into
*           nbytes - the number of bytes to copy
*   OUTPUTS: none
*   RETURN VALUE: 0 on success, -1 on failure
*   SIDE EFFECTS: none
*/
int32_t getargs (uint8_t* buf, uint32_t nbytes) {
    // Grab current pcb pointer
    pcb_t * pcb = get_curr_pcb_ptr();
    int i;

    // If there are no arguments in command line, return -1
    // Also need to check to make sure we have enough space on our buf for the command + NULL terminating character
    // So, command line can't be longer than 127
    if (strlen((const int8_t *) pcb->commands) == 0) return -1;
    
    // If there are more than 128 bytes for nbytes, we cap it off at the length of the command line
    // if (nbytes > strlen((const int8_t *) pcb->commands)) nbytes = strlen((const int8_t *) pcb->commands);
    // Loop through pcb commands and copy it to the buf
    for (i = 0; i < nbytes; i++) {
        if (i < strlen((const int8_t *) pcb->commands)) buf[i] = pcb->commands[i];
        else buf[i] = '\0';
    }

    return 0;
}

/*
* vidmap
*   DESCRIPTION: Maps the text-mode video memory into user space at a pre-set virtual address
*   INPUTS: screen_start - the address to map the video memory to
*   OUTPUTS: none
*   RETURN VALUE: 0 on success, -1 on failure
*   SIDE EFFECTS: changes paging structure
*/
int32_t vidmap (uint8_t** screen_start) {
    // check if screen start is valid 
    if (screen_start == NULL) return -1;

    if ((uint32_t) screen_start < USER_MEM_VIRTUAL_ADDR || (uint32_t) screen_start > (USER_MEM_VIRTUAL_ADDR + FOUR_MB)) return -1;

    // set up page as 4kb pages
    // video_memory_page_table[USER_VIDEO_MEM_INDEX].p = 1; 
    // video_memory_page_table[USER_VIDEO_MEM_INDEX].us = 1;
    // video_memory_page_table[USER_VIDEO_MEM_INDEX].base_31_12 = ; // (1 + get_terminal_idx());
    // flush_tlb();
    //(uint8_t *) (VIDEO + FOUR_KB * (1 + terminal_idx));  //
    // get_curr_pcb_ptr()->user_screen_start = screen_start;
    // get_curr_pcb_ptr()->screen_flag_set = 1;
    *screen_start = (uint8_t *) (VIDEO + FOUR_KB * (1 + get_schedule_idx())); 
    return 0;
}

/*
* set_handler
*   DESCRIPTION: Sets the handler for the given signal
*   INPUTS: signum - the signal to set the handler for
*           handler_address - the address of the handler
*   OUTPUTS: none
*   RETURN VALUE: -1
*   SIDE EFFECTS: none
*/
int32_t set_handler (uint32_t signum, void* handler_address) {
    return -1;
}

/*
* sigreturn
*   DESCRIPTION: Returns the signal to the handler
*   INPUTS: none
*   OUTPUTS: none
*   RETURN VALUE: -1
*   SIDE EFFECTS: none
*/
int32_t sigreturn (void) {
    return -1;
}
