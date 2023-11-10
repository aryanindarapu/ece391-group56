#include "process.h"

int32_t process_switch(int32_t process_num) {
    // process is currently 
    if (pcb_flags[process_num] == 0) {
        return -1;
    }


}