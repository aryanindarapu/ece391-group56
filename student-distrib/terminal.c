#include "terminal.h"
#include "lib.h"

uint8_t special_key_flags[NUM_SPECIAL_FLAGS] = { 0, 0, 0 };
static buffer_idx = 0;

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


void write_to_terminal(unsigned char keycode) {
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
        case 14:
            if(buffer_idx == 0) return;
            backspace();
            line_buffer[buffer_idx] = 0;
            buffer_idx--;
            return;
        case 28:
            printf("\n[Terminal]$ ");
            buffer_idx = 0;
            return;
        default:
            // TODO: may need to change bounds
            if(buffer_idx == 128) return;
            if (keycode > 0 && keycode < 58) {
                // Check if CTRL is held down
                
                if (special_key_flags[CTRL_INDEX]) {
                    switch (keycode) {
                    case 38:
                        
                    
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
                        line_buffer[buffer_idx] = shift_keyboard_map[keycode];
                        buffer_idx++;
                    }    
                    
                    else
                    {
                        putc(char_key);
                        line_buffer[buffer_idx] = char_key;
                        buffer_idx++;
                    }    
                        
                    
                    
                } else if (special_key_flags[SHIFT_INDEX]) {
                    putc(shift_keyboard_map[keycode]);
                    line_buffer[buffer_idx] = shift_keyboard_map[keycode];
                    buffer_idx++;
                }
                else
                {
                    putc(keyboard_map[keycode]);
                    line_buffer[buffer_idx] = keyboard_map[keycode];
                    buffer_idx++;
                }

            }

            // if(buffer_idx == 80)
            // {
            //     printf("\n");
            // }
                
            break;
    }

    
}

void terminal_clear() {
    clear();
    // TODO: clear buffer
}
