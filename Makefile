TOOL_CHAIN = arm-none-eabi-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -Iinclude
LDFLAGS 	:= -g

objs := start.o \
		entry.o \
		driver/imx_uart.o \
		device/qemu_print.o \
		driver/imx_gpio.o \
		driver/imx_i2c.o \
		device/at24cxx.o \
		driver/timer.o

6ul_bare_metal.bin: $(objs)
	${LD} -T 6ul_bare_metal.ld -o 6ul_bare_metal.elf $^
	${OBJCOPY} -O binary -S 6ul_bare_metal.elf $@
	${OBJDUMP} -D -m arm 6ul_bare_metal.elf > 6ul_bare_metal.dis


run: $(objs)
	${LD} -T 6ul_bare_metal.ld -o 6ul_bare_metal.elf $^
	${OBJCOPY} -O binary -S 6ul_bare_metal.elf 6ul_bare_metal.bin
	${OBJDUMP} -D -m arm 6ul_bare_metal.elf > 6ul_bare_metal.dis
	./qemu/bin/qemu-system-arm -M mcimx6ul-evk   -show-cursor  -m 512M -kernel 6ul_bare_metal.elf \
 	-display sdl -serial mon:stdio \
 	-nic user -com 100ask -device sd-card

debug: $(objs)
	${LD} -T 6ul_bare_metal.ld -o 6ul_bare_metal.elf $^
	${OBJCOPY} -O binary -S 6ul_bare_metal.elf 6ul_bare_metal.bin
	${OBJDUMP} -D -m arm 6ul_bare_metal.elf > 6ul_bare_metal.dis
	./qemu/bin/qemu-system-arm -M mcimx6ul-evk   -show-cursor  -m 512M -kernel 6ul_bare_metal.elf \
 	-display sdl -serial mon:stdio \
 	-nic user -com 100ask -device sd-card -S -s

%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC}  $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis driver/*.o
