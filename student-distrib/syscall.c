#include "syscall.h"
#include "file_system_driver.h"
#include "syscall_helpers.h"
#include "paging.h"
#include "lib.h"
#include "x86_desc.h"

int32_t execute (const uint8_t* command) {
    if (command == NULL) return -1;
    // Do iret to go to user memory
    // start program at given value in user memory

    // NOTE: all of this is probably out of order, need to rearrange
    
    // Assuming: filename arg1 arg2
    // NOTE: this doesn't allow for preceding spaces, but that's fine for now
    int i;
    uint8_t filename[128];
    for (i = 0; i < 128; i++) {
        if (command[i] == ' ' || command[i] == '\0') {
            filename[i] = '\0';
            break;
        }

        filename[i] = command[i];
    }

    // filename[idx + 1] = '\0';

    // TODO: check commands here, use getargs

    // TODO: copy file from kernel memory to user memory (need to set up 4 MB page)
    // this happens at 128 MB (find in doc)

    // also need to set up page that points to physical memory (8MB, then 12, then 16, ...)
    // this means the only thing we would change in the page is the table_base_addr --> use helper function

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
    // TODO: does startup set up a PCB on init?
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
    // TODO: add commands to PCB
    // TODO: do we need to keep track of EIP?
    pcb_t * new_pcb = (pcb_t *)(EIGHT_MB - (new_pid_idx + 1) * EIGHT_KB); // puts PCB pointer at bottom of kernel memory
    new_pcb->pid = new_pid_idx; // TODO: do i even need this field, can probably just use pid

    // TODO: fill in stdin and out to fd arr
    new_pcb->file_desc_arr[0].ops_ptr = stdin_ops_table;
    new_pcb->file_desc_arr[0].inode = -1;
    new_pcb->file_desc_arr[0].flags = 1;
    new_pcb->file_desc_arr[0].file_pos = 0;

    new_pcb->file_desc_arr[1].ops_ptr = stdout_ops_table;
    new_pcb->file_desc_arr[1].inode = -1;
    new_pcb->file_desc_arr[1].flags = 1;
    new_pcb->file_desc_arr[1].file_pos = 0;

    if (new_pid_idx == 0) { // i.e. the PCB is for the initial shell
        new_pcb->parent_pid = -1;
    } else {
        new_pcb->parent_pid = get_curr_pcb_ptr()->pid; // point to parent PCB pointer
    }

    /* Set up 4MB page for user program */
    page_dir_desc_t new_page_dir;
    new_page_dir.p = 1;
    new_page_dir.rw = 1; // TODO: check this
    new_page_dir.us = 1; // TODO: check this
    new_page_dir.pwt = 0;
    new_page_dir.pcd = 0;
    new_page_dir.a = 0;
    new_page_dir.d = 0;
    new_page_dir.ps = 1; // 1 - set to 4 MB page
    new_page_dir.g = 0; // TODO: check this, global flag
    new_page_dir.avail = 0;
    new_page_dir.table_base_addr = ((new_pid_idx * FOUR_MB) + EIGHT_MB) / FOUR_KB; // TODO: set to bottom of entry
    page_dir[USER_MEM_VIRTUAL_ADDR / FOUR_MB] = new_page_dir; // TODO: set to bottom of entry, also is the address start correct?
    flush_tlb();

    /* Copy to user memory */
    read_data(exec_dentry.inode_num, 0, (uint8_t *) PROGRAM_START, ((inode_t *)(inode_ptr + exec_dentry.inode_num))->length);
    
    /* Save regs needed for PCB */
    // Read bytes 24 - 27 to get eip
    uint8_t eip_ptr[sizeof(uint32_t)];
    read_data(exec_dentry.inode_num, EIP_START, eip_ptr, sizeof(uint32_t));

    // new_pcb->eip_reg = *((uint32_t *) eip_ptr); // TODO: check if this is correct
    // new_pcb->esp_reg = 

    /* Set up TSS */ // TSS - contains process state information of the parent task to restore it
    tss.ss0 = (uint16_t) KERNEL_DS; // segment selector for kernel data segment
    tss.esp0 = (uint32_t) new_pcb + EIGHT_KB - STACK_FENCE_SIZE; // offset of kernel stack segment -- TODO: idk what this should be
    
    uint32_t user_eip = *((uint32_t *) eip_ptr);
    uint32_t user_esp = PROGRAM_START - STACK_FENCE_SIZE; // TODO: how is this something that we arrived at?

    /* push the user eip and esp to the cur pcb so we can restore context */
    new_pcb->user_space_eip = user_eip;
    new_pcb->user_space_esp = user_esp;

    // asm volatile("movl %%esp, %0" : "=r" (esp));
    // asm volatile("movl %%ebp, %0" : "=r" (ebp));
    // new_pcb->kern_esp = esp;
    // new_pcb->kern_ebp = ebp;
    // eh idk 

    // TODO: store kernel esp
    asm volatile (
        "movl %%esp, %%eax   ;\
         movl %%ebp, %%ebx   ;\
        "
        : "=a" (new_pcb->kernel_esp), "=b" (new_pcb->kernel_ebp)
        :
        : "memory"
    );

    // sti();
    // TODO BEN: add iret, asm volatile here
    // set up DS, ESP, EFLAGS, CS, EIP
    uint32_t output;
    // https://stackoverflow.com/questions/6892421/switching-to-user-mode-using-iret

    asm volatile ("\
        andl $0x00FF, %%eax     ;\
        movw %%ax, %%ds         ;\
        pushl %%eax             ;\
        pushl %%ebx             ;\
        pushfl                  ;\
        popl %%ebx              ;\
        orl $0x0200, %%ebx      ;\
        pushl %%ebx             ;\
        pushl %%ecx             ;\
        pushl %%edx             ;\
        iret                    ;\
        "
        : "=a" (output)
        : "a" (USER_DS), "b" (user_esp), "c" (USER_CS), "d" (user_eip) 
        : "memory"
    );

    asm volatile ("ret_from_halt: \n");
    return 0;
}

/* NO NEED TO IMPLEMENT YET(CHECKPOINT 3.2 COMMENT) */
int32_t halt (uint8_t status) {
    // When closing, do I need to check if current PCB has any child PCBs?
    pcb_t * pcb = get_curr_pcb_ptr();

    /* push user context if its base shell */
    if (pcb->pid == 0) {
        uint32_t esp = pcb->user_space_esp;
        uint32_t eip = pcb->user_space_eip;
        // recover context from halt(esp, eip, USER_CS, USER_DS);
        asm volatile ("\
            andl    $0x00FF, %%ebx  ;\
            movw    %%bx, %%ds      ;\
            pushl   %%ebx           ;\
            pushl   %%edx           ;\
            pushfl                  ;\
            popl    %%edx           ;\
            orl     $0x0200, %%edx  ;\
            pushl   %%edx           ;\
            pushl   %%ecx           ;\
            pushl   %%eax           ;\
            iret                    ;\
            "
            :
            : "a"(eip), "b"(USER_DS), "c"(USER_CS), "d"(esp)
            : "memory"
        );
    }

    pcb_t * parent_pcb = get_pcb_ptr(pcb->parent_pid);

    // remove everything from FD array
    int i;
    for (i = 0; i < MAX_FILE_DESC; i++) {
        pcb->file_desc_arr[i].flags = 0;
    } 

    // remove current pcb from 
    pcb_flags[pcb->pid] = 0;
    
    // Set TSS again
    tss.ss0 = (uint16_t) KERNEL_DS; // segment selector for kernel data segment
    tss.esp0 = (uint32_t) parent_pcb + EIGHT_KB - STACK_FENCE_SIZE;

    // Change physical memory page address
    // page_dir[USER_MEM_VIRTUAL_ADDR / FOUR_MB].table_base_addr = (((parent_pcb->pid) * FOUR_MB) + EIGHT_MB) / FOUR_KB;

    /* Restore parent paging */
    page_dir[USER_MEM_VIRTUAL_ADDR / FOUR_MB].table_base_addr = ((PHYSICAL_MEMORY + pcb->parent_pid) * FOUR_MB)/FOUR_KB;
    flush_tlb(); // need to update the paging structure

    end_halt(pcb->kernel_ebp, pcb->kernel_esp, status);

    //if we get control back then we return fail(we shouldn't ever get control back)
    return -1;
}

// TODO: this seems to do the exact same things as file open and dir open, need to check for overlap
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
            return 0;
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

int32_t close (uint32_t fd) {
    if (fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    pcb_t * pcb = get_curr_pcb_ptr();
    if (!pcb->file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return pcb->file_desc_arr[fd].ops_ptr.close(fd);
}

int32_t read (uint32_t fd, void* buf, uint32_t nbytes) {
    if (fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    pcb_t * pcb = get_curr_pcb_ptr();
    if (!pcb->file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return pcb->file_desc_arr[fd].ops_ptr.read(fd, buf, nbytes);
}

int32_t write (uint32_t fd, const void* buf, uint32_t nbytes) {
    if (fd >= MAX_FILE_DESC) return -1; // Checks if fd is 0 or 1
    pcb_t * pcb = get_curr_pcb_ptr();
    if (!pcb->file_desc_arr[fd].flags) return -1; // Checks if fd is inactive
    return pcb->file_desc_arr[fd].ops_ptr.write(fd, buf, nbytes);
}


/* NO NEED TO IMPLEMENT YET (CHECKPOINT 3.4)*/
int32_t vidmap (uint8_t** screen_start) {
    return -1;
}

int32_t getargs (uint8_t* buf, uint32_t nbytes) {
    return -1;
}

/* EXTRA CREDIT SYSTEM CALLS */
int32_t set_handler (uint32_t signum, void* handler_address) {
    return -1;
}

int32_t sigreturn (void) {
    return -1;
}
