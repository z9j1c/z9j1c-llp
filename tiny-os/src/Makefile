CPREFIX = $(HOME)/opt/cross/bin
STDLIBSRC = stdlib/src

AS=$(CPREFIX)/i686-elf-gcc -m32 -c -g -mgeneral-regs-only -mno-red-zone
CC=$(CPREFIX)/i686-elf-gcc -m32 -g -mgeneral-regs-only -mno-red-zone
LD=$(CPREFIX)/i686-elf-gcc -m32

### KERNEL targets
build: kernel.bin clean

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

interrupts.o: interrupts.c
	$(CC) -c interrupts.c -o interrupts.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

kernel.bin: kernel.o boot.o interrupts.o string.o
	$(LD) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o interrupts.o kernel.o string.o -lgcc

### STDLIB targets

string.o: stdlib/src/string.c
	$(CC) -c $(STDLIBSRC)/string.c -o string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

clean:
	rm *.o

.PHONY: build
