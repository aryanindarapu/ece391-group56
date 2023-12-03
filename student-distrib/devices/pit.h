#ifndef _PIT_H
#define _PIT_H

#include "../types.h"

#define PIT_CHANNEL0_DATA   0x40
#define PIT_CHANNEL1_DATA   0x41
#define PIT_CHANNEL2_DATA   0x42
#define PIT_COMMAND         0x43

// Sends an interupt to the PIC to let PIC know we're doing a process switch
int pit_handler();//int eip, int esp);

// Intializes the pit on the PIC
void init_pit();

// Grabs the scheduled index
int get_schedule_idx();

// Sets the scheduling index
void set_schedule_idx(int index);

// Check to see if our terminals are done being set up
int is_terminals_initialized();

#endif
