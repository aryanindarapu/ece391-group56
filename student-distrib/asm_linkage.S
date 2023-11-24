/* these don't push irq num?*/

#define INTR_LINK(name,func)   \
.GLOBL name                 ;\
name:                       ;\
    pushal                  ;\
    pushfl                  ;\
    call func               ;\
    popfl                   ;\
    popal                   ;\
    iret                    ;\

#define INTR_LINK_PIT(pname,pfunc)   \
.GLOBL pname                 ;\
pname:                       ;\
    pushal                  ;\
    pushfl                  ;\
    pushl %esp              ;\
    call pfunc               ;\
    popl %esp               ;\
    popfl                   ;\
    popal                   ;\
    iret                    ;\



/* link device handlers */
INTR_LINK(keyboard_handler_linkage, keyboard_handler)
INTR_LINK(rtc_handler_linkage, rtc_handler)
INTR_LINK_PIT(pit_handler_linkage, pit_handler)
