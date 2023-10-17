/*
*      KEYBOARD HANDLER FILE
*      INTIALIZE KEYBOARD AND READ INPUT
*
*/

#include "../lib.h"
#include "../x86_desc.h"
#include "keyboard.h"

/*
 *   init_keyboard
 *   DESCRIPTION: initialization for keyboard on PIC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: enables IRQ1 on PIC for the keyboard
 */ 
void init_keyboard() {
    // Let PIC enable the IRQ1 for keyboard
    enable_irq(1);
}

/*
 *   keyboard_handler
 *   DESCRIPTION: This is the handler for the keyboard interrupts. It reads the data from port x60 and sends an EOI
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints any keyboard input to the screen
 */ 
void keyboard_handler() {
    // Read input from keyboard
    read_keyboard();
    
    // Signal that interrupt is done
    send_eoi(1);
}

// TODO: move this to terminal.c (for terminal driver). this driver should send the buffer to a function here, which then parses it and does whatever ops it needs to.
/*
 *   read_keyboard
 *   DESCRIPTION: helper fnc to read data from keyboard port x60
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints the data in the keyboard to screen
 */ 
void read_keyboard () {
    
    //printf("keyboard int occured\n");
    unsigned char status;
    unsigned char keycode;
    // Control key code: 29
    
    // Created a keyboard map correlating key press values to characters to display on screen
    // TODO: move this to a const in keyboard.h
    unsigned char keyboard_map[128] =
    {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
    '9', '0', '-', '=', '\b',     /* Backspace */
    '\t',                 /* Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
        0,                  /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',     /* 39 */
    '\'', '`',   0,                /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
    'm', ',', '.', '/',   0,                              /* Right shift */
    '*',
        0,  /* Alt */
    ' ',  /* Space bar */
        0,  /* Caps lock */
        0,  /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  /* < ... F10 */
        0,  /* 69 - Num lock*/
        0,  /* Scroll Lock */
        0,  /* Home key */
        0,  /* Up Arrow */
        0,  /* Page Up */
    '-',
        0,  /* Left Arrow */
        0,
        0,  /* Right Arrow */
    '+',
        0,  /* 79 - End key*/
        0,  /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        0,  /* Delete Key */
        0,   0,   0,
        0,  /* F11 Key */
        0,  /* F12 Key */
        0,  /* All other keys are undefined */
    };
    
    // Include status to check for letting go character (unsure fully if needed)
    status = inb(0x64);  
    if (status & 0x01) {
        // Read the keyboard value at PORT 0x60
        keycode = inb(0x60);  
        // MP3.1: Check to see if it is a valid character or number
        printf("Keycode is: %d", keycode);
        if (keycode > 0 && keycode < 58) {
            // Print the character using the keycode as an index
            putc(keyboard_map[keycode]);

            // Put character into buffer
            // keyboard_buffer[cursor_position] = keycode;
            // cursor_position++;
        } else {
            if (keycode == CTRL_KEYCODE) {
                // flip ctrl flag
                
            }
        }
    }
}
