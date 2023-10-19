/*
*      KEYBOARD HANDLER FILE
*      INTIALIZE KEYBOARD AND READ INPUT
*
*/

#include "../lib.h"
#include "../x86_desc.h"
#include "keyboard.h"
#include "../terminal.h"        

uint8_t special_key_flags[NUM_SPECIAL_FLAGS] = { 0, 0, 0 };

// TODO: remove static, idk how, keyboard.c seems to give error
const unsigned char keyboard_map[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',    // 0 - 14
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     // 15 - 28
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',    // 29 - 43
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',                           // 44 - 53
        0, 
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
    
const unsigned char shift_keyboard_map[128] = 
    {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',    // 0 - 14
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     // 15 - 28
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',    // 29 - 43
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',                           // 44 - 53
        0, 
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
    keyboard_driver();
    
    // Signal that interrupt is done
    send_eoi(1);
}

// TODO: comment
void keyboard_driver() {
    // cli();
    //printf("keyboard int occured\n");
    //unsigned char status;
    unsigned char keycode = inb(KEYBOARD_PORT);  
    // TODO: do all special key checking here
    // Control key code: 29
    // Set special 

    switch (keycode) {
        case CTRL_P:
            special_key_flags[CTRL_INDEX] = 1;
            return;
        case CTRL_R:
            special_key_flags[CTRL_INDEX] = 0;
            return;
        case SHIFT_P:
            special_key_flags[SHIFT_INDEX] = 1;
            return;
        case SHIFT_R:
            special_key_flags[SHIFT_INDEX] = 0;
            return;
        case CAPS_LOCK_P:
            if (special_key_flags[CAPS_LOCK_INDEX] == 0) {
                special_key_flags[CAPS_LOCK_INDEX] = 1;
            } else {
                special_key_flags[CAPS_LOCK_INDEX] = 0;
            }
            return;
        case 14: //backspace TODO : just sent to terminal to enter into buffer, handle it there for backspace, save enter for a read
            if(get_buffer_fill() == 0) return;
            backspace();
            terminal_backspace();
            //printf("%d",get_buffer_fill());
            return;
        case 28: //enter
            printf("\n[Terminal]$ ");
            terminal_clear();
            return;
        default:

            // TODO: may need to change bounds
            if(get_buffer_fill() == 128) return; //do check in terminal
            if (keycode > 0 && keycode < 58) {
                // Check if CTRL is held down
                
                if (special_key_flags[CTRL_INDEX]) {
                    switch (keycode) {
                    case 38:
                        clear();
                        terminal_clear();
                        break;
                    default:
                        break;
                    }
                } else if (special_key_flags[CAPS_LOCK_INDEX]) {
                    
                    char char_key = keyboard_map[keycode];
                    if (char_key > 96 && char_key < 123 && special_key_flags[SHIFT_INDEX] == 0)
                    {
                        putc(shift_keyboard_map[keycode]);
                        write_to_terminal(shift_keyboard_map[keycode]);
                        //line_buffer[buffer_idx] = shift_keyboard_map[keycode];
                        //buffer_idx++;
                    }    
                    
                    else
                    {
                        putc(char_key);
                        write_to_terminal(char_key);
                        // line_buffer[buffer_idx] = char_key;
                        // buffer_idx++;
                    }    
                        
                    
                    
                } else if (special_key_flags[SHIFT_INDEX]) {
                    putc(shift_keyboard_map[keycode]);
                    write_to_terminal(shift_keyboard_map[keycode]);
                    // line_buffer[buffer_idx] = shift_keyboard_map[keycode];
                    // buffer_idx++;
                }
                else
                {
                    putc(keyboard_map[keycode]);
                    write_to_terminal(keyboard_map[keycode]);
                    // line_buffer[buffer_idx] = keyboard_map[keycode];
                    // buffer_idx++;
                }

            }

            // if(buffer_idx == 80)
            // {
            //     printf("\n");
            // }
                
            break;
    }

    // Created a keyboard map correlating key press values to characters to display on screen
    // TODO: move this to a const in keyboard.h
    // write_to_terminal(keycode);
    // }
    // sti();
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
// void read_keyboard () {
    
//     //printf("keyboard int occured\n");
//     unsigned char status;
//     unsigned char keycode;
//     // Control key code: 29
    
//     // Created a keyboard map correlating key press values to characters to display on screen
//     // TODO: move this to a const in keyboard.h
//     unsigned char keyboard_map[128] =
//     {
//         0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
//     '9', '0', '-', '=', '\b',     /* Backspace */
//     '\t',                 /* Tab */
//     'q', 'w', 'e', 'r',   /* 19 */
//     't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
//         0,                  /* 29   - Control */
//     'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',     /* 39 */
//     '\'', '`',   0,                /* Left shift */
//     '\\', 'z', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
//     'm', ',', '.', '/',   0,                              /* Right shift */
//     '*',
//         0,  /* Alt */
//     ' ',  /* Space bar */
//         0,  /* Caps lock */
//         0,  /* 59 - F1 key ... > */
//         0,   0,   0,   0,   0,   0,   0,   0,
//         0,  /* < ... F10 */
//         0,  /* 69 - Num lock*/
//         0,  /* Scroll Lock */
//         0,  /* Home key */
//         0,  /* Up Arrow */
//         0,  /* Page Up */
//     '-',
//         0,  /* Left Arrow */
//         0,
//         0,  /* Right Arrow */
//     '+',
//         0,  /* 79 - End key*/
//         0,  /* Down Arrow */
//         0,  /* Page Down */
//         0,  /* Insert Key */
//         0,  /* Delete Key */
//         0,   0,   0,
//         0,  /* F11 Key */
//         0,  /* F12 Key */
//         0,  /* All other keys are undefined */
//     };
    
//     // Include status to check for letting go character (unsure fully if needed)
//     status = inb(0x64);  
//     if (status & 0x01) {
//         // Read the keyboard value at PORT 0x60
//         keycode = inb(KEYBOARD_PORT);  
//         // MP3.1: Check to see if it is a valid character or number
//         printf("Keycode is: %d", keycode);
//         if (keycode > 0 && keycode < 58) {
//             // Print the character using the keycode as an index
//             putc(keyboard_map[keycode]);

//             // Put character into buffer
//             // keyboard_buffer[cursor_position] = keycode;
//             // cursor_position++;
//         }
//     }
// }
