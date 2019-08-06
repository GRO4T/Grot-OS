# Automatically generate lists of sources using wildcards
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

#Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o}

# Default build target
all: os-image

run: all
	qemu-system-x86_64 -fda os-image

os-image: boot/boot_sect.bin kernel/kernel.bin
	cat $^ > os-image

# Build the kernel binary
# $^ is substituted with all of the target's dependancy files
kernel/kernel.bin : kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# Generic rule for compiling C code to an object file
# For simplicity, C files depend on all header files
%.o : %.c ${HEADERS}
	gcc -m32 -fno-pic -ffreestanding -c $< -o $@

# Assemble the kernel entry
%.o : %.asm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
%.bin : %.asm
	nasm $< -f bin -I '16bit/' -o $@

clean:
	rm -rf *.bin *.o *.dis os-image
	rm -rf kernel/*.o boot/*.bin drivers/*.o

# Disassemble our kernel - might be useful for debugging.
kernes.dis : kernel.bin
	ndisasm -b 32 $< > $@
