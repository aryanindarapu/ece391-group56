#define INTR_LINK(name,func)   \
.GLOBL name                 ;\
name:                       ;\
    pushal                  ;\
    pushfl                  ;\
    movl 36(%esp), %eax     ;\
    movl 48(%esp), %ebx     ;\
    call func               ;\
    popfl                   ;\
    popal                   ;\
    iret                    ;\

/* link device handlers */
INTR_LINK(keyboard_handler_linkage, keyboard_handler)
INTR_LINK(rtc_handler_linkage, rtc_handler)
INTR_LINK(pit_handler_linkage, pit_handler)
