TOOL_CHAIN = arm-none-eabi-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -Iinclude
LDFLAGS 	:= -g

objs := start.o entry.o imx_uart.o qemu_print.o

6ul_bare_metal.bin: $(objs)
	${LD} -T 6ul_bare_metal.ld -o 6ul_bare_metal.elf $^
	${OBJCOPY} -O binary -S 6ul_bare_metal.elf $@
	${OBJDUMP} -D -m arm 6ul_bare_metal.elf > 6ul_bare_metal.dis


run: $(objs)
	${LD} -T 6ul_bare_metal.ld -o 6ul_bare_metal.elf $^
	${OBJCOPY} -O binary -S 6ul_bare_metal.elf 6ul_bare_metal.bin
	${OBJDUMP} -D -m arm 6ul_bare_metal.elf > 6ul_bare_metal.dis
	../ubuntu-18.04_imx6ul_qemu_system/qemu/bin/qemu-system-arm -M mcimx6ul-evk   -show-cursor  -m 512M -kernel 6ul_bare_metal.elf \
 	-display sdl -serial mon:stdio \
 	-nic user -com 100ask -device sd-card

debug: $(objs)
	${LD} -T 6ul_bare_metal.ld -o 6ul_bare_metal.elf $^
	${OBJCOPY} -O binary -S 6ul_bare_metal.elf 6ul_bare_metal.bin
	${OBJDUMP} -D -m arm 6ul_bare_metal.elf > 6ul_bare_metal.dis
	../ubuntu-18.04_imx6ul_qemu_system/qemu/bin/qemu-system-arm -M mcimx6ul-evk -m 512M -nographic -kernel 6ul_bare_metal.elf -serial mon:stdio -s -S

%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC}  $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
