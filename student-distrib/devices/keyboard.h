#include "i8259.h"

// Intialize the keyboard
void init_keyboard();

// Read the value from the keyboard
void read_keyboard();

// For when the interrupt occurs
void keyboard_handler();
