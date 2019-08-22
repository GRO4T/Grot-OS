# Compilation setup
# i386-elf cross compiler
CC = /home/damian/opt/cross/bin/i386-elf-gcc
# SUPPRESS THIS WARNINGS:
# 		pointer-to-int-cast
# 		int-conversion
CFLAGS = -ffreestanding
LD = /home/damian/opt/cross/bin/i386-elf-ld

# Automatically generate lists of sources using wildcards
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)

#Convert the *.c filenames to *.o to give a list of object files to build
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

# Default target
release:

ifneq (,$(wildcard ./DEBUG)) # if DEBUG exist clean
release: clean release/os-image
	rm DEBUG
else
release: release/os-image
endif

# Run qemu emulator
run: release
	qemu-system-i386 -fda release/os-image

# Build OS image
release/os-image: boot/boot_sect.bin kernel/kernel.bin
	@mkdir -p $(@D)
	cat $^ > release/os-image

# Build the kernel binary
# $^ is substituted with all of the target's dependancy files
kernel/kernel.bin : kernel/kernel_entry.o ${OBJ}
		 $(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

# Generic rule for compiling C code to an object file
# For simplicity, C files depend on all header files
%.o : %.c ${HEADERS}
		 $(CC) $(CFLAGS) -c $< -o $@

# Assemble the kernel entry
%.o : %.asm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
%.bin : %.asm
	nasm $< -f bin -I '16bit/' -o $@

clean:
	rm -rf release/* debug/*
	rm -rf kernel/*.o  ${OBJ}
	rm -rf kernel/kernel.bin boot/*.bin

# Disassemble our kernel - might be useful for debugging.
kernel.dis : kernel/kernel.bin
	ndisasm -b 32 $< > $@


# Phony targets
#.PHONY: clean debug release run run_debug


#----------------------------- DEBUG ---------------------------------------------#

# Build OS image and
debug: CFLAGS += -g -H
debug: debug/os-image
#checking whether DEBUG flag exists
ifeq (,$(wildcard ./DEBUG)) # if it doesn't already exist create file named DEBUG
	touch DEBUG   				# simply signaling whether last build was debug or release
endif

# Debug version of the build
debug/os-image: boot/boot_sect.bin kernel/kernel.bin
	@mkdir -p $(@D)
	cat $^ > debug/os-image

# run qemu and listen for gdb connection on port 1234
# 		-s shorthand for -gdb tcp::1234, i.e. open gdbserver on TCP port 1234
# 		-S do not start CPU at startup
#
# After qemu starts listening:
#		open a terminal then type:
#			gdb
#			(gdb) target remote localhost:1234
run_debug: debug
	qemu-system-x86_64 -s- -S -fda release/os-image

#-------------------------------- TEST -----------------------------------------#
#TEST_C_SOURCES = $(wildcard test/*.c drivers/*.c)
#TEST_OBJ = ${TEST_C_SOURCES:.c=.o}
#
#
#test/test-image: boot/boot_sect.bin test/test_kernel.bin
#	cat $^ > $@
#
#test/test_kernel.bin: kernel/kernel_entry.o ${TEST_OBJ} kernel/util.o
#	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
#
#run_test: test/test-image
#	qemu-system-x86_64 -fda $<
