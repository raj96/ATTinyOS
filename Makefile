PROJECT=ATTinyOS

AS=avr-as -mmcu=avr25
CC=avr-gcc
CFLAGS=-Wall -Werror -O2 -mmcu=avr25 -nostartfiles -nostdlib
IFLAGS=-I os/include
LFLAGS=-T link.ld -Xlinker -Map -Xlinker $(PROJECT).map

MCN=/home/raja/.local/bin/micronucleus

OBJS=entry.o
OBJS+=vectors.o
OBJS+=os/kmain.o
OBJS+=os/lib/scheduler/tasker.o
OBJS+=os/lib/scheduler/switch.o

$(PROJECT).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $(PROJECT).elf

flash: $(PROJECT).elf
	avr-objcopy -O ihex $(PROJECT).elf $(PROJECT).hex
	$(MCN) --no-ansi --run --timeout 30 $(PROJECT).hex

info:
	avr-size --mcu=attiny85 -C $(PROJECT).elf

clean:
	rm -f $(OBJS)
	rm -f $(PROJECT).elf
	rm -f $(PROJECT).hex
	rm -f $(PROJECT).s
	rm -f $(PROJECT).map

dump:
	avr-objdump -D $(PROJECT).elf > $(PROJECT).s

%.o: %.c 
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

lib/%.o: include/%.h

%.o: %.s
	$(AS) $< -o $@
