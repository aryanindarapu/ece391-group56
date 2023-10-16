#define INTR_LINK(name,func)   \
.GLOBL name                 ;\
name:                       ;\
    pushal                  ;\
    pushfl                  ;\
    call func               ;\
    popfl                   ;\
    popal                   ;\
    iret                    ;\

/* the system call library wrappers */
INTR_LINK(keyboard_handler_linkage, keyboard_handler)
INTR_LINK(rtc_handler_linkage, rtc_handler)
