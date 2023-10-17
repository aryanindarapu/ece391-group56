#include "i8259.h"
#include "keyboard_driver.h"

#define NUM_SPECIAL_FLAGS 5  // enter, ctrl, alt, shift, caps lock

// TODO: 
#define CTRL_KEYCODE 29

// Intialize the keyboard
void init_keyboard();

// Read the value from the keyboard
void read_keyboard();

// For when the interrupt occurs
void keyboard_handler();

// TODO: may need to add cli, sti since this is a crit section
uint8_t special_key_flags[NUM_SPECIAL_FLAGS] = { 0, 0, 0, 0, 0 }; 