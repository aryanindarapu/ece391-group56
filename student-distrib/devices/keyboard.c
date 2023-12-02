/*
*      KEYBOARD HANDLER FILE
*      INTIALIZE KEYBOARD AND READ INPUT
*
*/

#include "../lib.h"
#include "../x86_desc.h"
#include "keyboard.h"
#include "../terminal.h"   

extern int new_terminal_flag;
extern int temp_terminal_flag;

uint8_t special_key_flags[NUM_SPECIAL_FLAGS] = { 0, 0, 0, 0 };

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
    // while (new_terminal_flag) {}
    // Read input from keyboard
    keyboard_driver();
    
    // while (!temp_terminal_flag) {};
    // Signal that interrupt is done
    send_eoi(1);
}

/* keyboard_driver
 * Inputs: none
 * Return Value: none
 * Function: reads in from keyboard port and decodes keycode. Sends necessary data to terminal to update the buffer */
void keyboard_driver() {
    // TODO: leave if new term flag set, we need wait for term to start
    if(new_terminal_flag == 1) return;
    int term = get_terminal_idx();
    //printf("keyboard int occured\n");
    //unsigned char status;
    // give to active terminal the keycoamdndadaADadadasdadadadadaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    unsigned char keycode = inb(KEYBOARD_PORT);  
    // Control key code: 29
    // Set special 
    //printf("%d",keycode);
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
        case ALT_P:
            /* assert alt pressed flag */
            special_key_flags[ALT_INDEX] = 1;
            return;
        case ALT_R:
            /* release alt flag */
            special_key_flags[ALT_INDEX] = 0;
            return;
        case BACKSPACE:
            if(get_buffer_fill() == 0) return;
            // backspace();
            terminal_backspace();
            //printf("%d",get_buffer_fill());
            return;
        case ENTER: //enter
            //if(get_buffer_fill() == 128) return;
            putc_terminal('\n', term);
            //printf("\n");//printf("\n[Terminal]$ ");
            terminal_enter();
            return;
        case TAB:
            if(get_buffer_fill() == 127) return;
            putc_terminal(' ', term);
            putc_terminal(' ', term);
            putc_terminal(' ', term);
            putc_terminal(' ', term);
            // putc_kbd(' ', get_terminal_idx()); // put 4 spaces for tab
            // putc_kbd(' ', get_terminal_idx());
            // putc_kbd(' ', get_terminal_idx());
            // putc_kbd(' ', get_terminal_idx());
            write_to_terminal(keyboard_map[keycode]);
            return;
        default:
            if(get_buffer_fill() == 127) return; //do check in terminal
            if (keycode > 0 && keycode < 58) {
                // Check if CTRL is held down
                if (special_key_flags[CTRL_INDEX]) {
                    switch (keycode) {
                    case 38:
                        // lib stuff
                        clear_terminal(term);
                        // buffer stuff
                        terminal_clear();
                        break;
                    default:
                        break;
                    }
                } else if (special_key_flags[CAPS_LOCK_INDEX]) {
                    
                    char char_key = keyboard_map[keycode];
                    if (char_key > 96 && char_key < 123 && special_key_flags[SHIFT_INDEX] == 0)
                    {
                        putc_terminal(shift_keyboard_map[keycode], term);
                        write_to_terminal(shift_keyboard_map[keycode]);
                        //line_buffer[buffer_idx] = shift_keyboard_map[keycode];
                        //buffer_idx++;
                    }    
                    
                    else
                    {
                        putc_terminal(char_key, term);
                        write_to_terminal(char_key);
                        // line_buffer[buffer_idx] = char_key;
                        // buffer_idx++;
                    }    
                        
                    
                    
                } else if (special_key_flags[SHIFT_INDEX]) {
                    putc_terminal(shift_keyboard_map[keycode], term);
                    write_to_terminal(shift_keyboard_map[keycode]);
                    // line_buffer[buffer_idx] = shift_keyboard_map[keycode];
                    // buffer_idx++;
                }
                else
                {
                    putc_terminal(keyboard_map[keycode], term);
                    write_to_terminal(keyboard_map[keycode]);
                    // line_buffer[buffer_idx] = keyboard_map[keycode];
                    // buffer_idx++;
                }

            }
            else if(keycode > 58 && keycode < 62){
                // quick test to detect alt + Function key
                if (special_key_flags[ALT_INDEX]){
                    terminal_switch(keycode - 59);
                    //printf("Switched to terminal ");
                    //putc_kbd(keyboard_map[2] + keycode - 59);
                    //write_to_terminal(keyboard_map[2] + keycode - 59);
                }
            }

            // if(buffer_idx == 80)
            // {
            //     printf("\n");
            // }
                
            break;
    }
}
