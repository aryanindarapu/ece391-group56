# KerML: A Linux-Based Kernel Simplified With ML
Built off of UIUC's ECE 391 Kernel OS. The OS was emulated on Qemu on x86-32 archiecture. A demo can be seen here:

## List of Functionalities
The following functionalities have been implemented during the development of this OS:
- Initializing GDT, IDT, and TSS
- Read-only Ext2-like filesystem
- Several devices connected to the PIC (including RTC, keyboard, and PIT)
- Hardware interrupts and signals
- A reduced set of system calls
- Ability to open multiple processes/tasks
- Multiple terminal switching (multi-threading)
- Round robin scheduling
- Dynamic memory allocation
- Naive Bayes algorithm that converts natural language to kernel commands

### Natural Language Kernel Commands
This kernel implements a Naive Bayes algorithm that takes natural language input from the terminal and exectues the command that the user intends to. For example, take the following input:

    Print out the contents of frame1.txt.

The kernel would then run `cat frame1.txt`.
