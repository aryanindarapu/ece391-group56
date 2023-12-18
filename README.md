# KerML: Integrating a Linux Kernel with NLP
Built off of UIUC's ECE 391 Kernel OS. The OS is a based off of Linux (a very early version) and implements several basic systems concepts. It can be emulated on Qemu and uses x86-32 architecture. A demo can be seen here:

## List of Features
The following features have been implemented during the development of this OS:
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
