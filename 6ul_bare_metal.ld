ENTRY(reset)
SECTIONS
{
	. = 0x00000000;
 	.startup . : { start.o(.text) }
 	.text : { *(.text) }
    . = 0x00900000;
 	.data : { *(.data) }
 	.bss : { *(.bss COMMON) }
 	. = ALIGN(8);
 	. = . + 0x8000; /* 32kB of stack memory */
 	svc_stack_top = .;
}
