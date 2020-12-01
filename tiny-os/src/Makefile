CPREFIX = $(HOME)/opt/cross/bin
STDLIBSRC = stdlib/src

AS=i686-elf-gcc -m32 -c -g -mgeneral-regs-only -mno-red-zone
CC=i686-elf-gcc -m32 -g -mgeneral-regs-only -mno-red-zone
LD=i686-elf-gcc -m32

KERNEL_SRC=$(wildcard *.c)
STDLIB_SRC=$(wildcard stdlib/src/*.c)
KERNEL_OBJ=$(KERNEL_SRC:%.c=%.o)
STDLIB_OBJ=$(STDLIB_SRC:stdlib/src/%.c=%.o)

### KERNEL targets
build: kernel.bin clean

boot.o: boot.s
	$(AS) boot.s -o boot.o

# kernel.o: kernel.c
# 	$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

all_c_build: $(KERNEL_SRC)
	$(CC) -c $(KERNEL_SRC) $(STDLIB_SRC) -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# interrupts.o: interrupts.c
# 	$(CC) -c interrupts.c -o interrupts.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

kernel.bin: all_c_build boot.o
	$(LD) -T linker.ld -o kernel.bin -ffreestanding -O2 -nostdlib boot.o $(KERNEL_OBJ) $(STDLIB_OBJ) -lgcc

### STDLIB targets

# string.o: stdlib/src/string.c
# 	$(CC) -c $(STDLIBSRC)/string.c -o string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

clean:
	rm *.o

.PHONY: build
