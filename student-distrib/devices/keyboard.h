#include "i8259.h"


#define NUM_SPECIAL_FLAGS 3

#define CAPS_LOCK_P 58
#define CTRL_P 29
#define CTRL_R 157
#define SHIFT_P 42
#define SHIFT_R 170

#define CTRL_INDEX 0 
#define SHIFT_INDEX 1
#define CAPS_LOCK_INDEX 2
// #define ALT_INDEX 2 


// Intialize the keyboard
void init_keyboard();

// Read the value from the keyboard
// void read_keyboard();

// For when the interrupt occurs
void keyboard_handler();
// handles reading and interaction with temrinal
void keyboard_driver();
