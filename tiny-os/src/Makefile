AS=gcc -m32 -c -g -mgeneral-regs-only -mno-red-zone
CC=gcc -m32 -g -mgeneral-regs-only -mno-red-zone
LD=gcc -m32

KERNEL_SRC=$(wildcard *.c)
STDLIB_SRC=$(wildcard stdlib/src/*.c)
KERNEL_OBJ=$(KERNEL_SRC:%.c=%.o)
STDLIB_OBJ=$(STDLIB_SRC:stdlib/src/%.c=%.o)


build: kernel.bin clean

boot.o: boot.s
	$(AS) boot.s -o boot.o


gdt_asm.o: gdt.s
	$(AS) -c gdt.s -o gdt_asm.o


all_c_build: $(KERNEL_SRC) $(STDLIB_SRC)
	$(CC) -c $(KERNEL_SRC) $(STDLIB_SRC) -std=gnu99 -ffreestanding -O2 -Wall -Wextra


kernel.bin: all_c_build boot.o gdt_asm.o
	$(LD) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o gdt_asm.o $(KERNEL_OBJ) $(STDLIB_OBJ) -lgcc


clean:
	rm *.o


.PHONY: build
