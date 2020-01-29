.align 4
.global reset
.global c_entry
.section .isr_vector
.text
reset:
	B reset_handler
	B .
	B .             //SVC
	B .
	B .
	B .
	B .             //IRQ
	B .             //FIQ


reset_handler:
    ldr r0, =0x00900000
    mcr p15,0,r0,c12,c0,0
    ldr sp, =svc_stack_top
    bl c_entry
    b .
 