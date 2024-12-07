# Compiler and flags
CC = i686-elf-gcc
AS = i686-elf-as
CFLAGS = -ffreestanding -O2 -nostdlib
LDFLAGS = -T linker.ld
OBJS = boot.o system.o screen.o vsprintf.o descriptor_tables.o interrupt.o timer.o main.o

# Output binary
OUTPUT = tinyos.bin

# Default target
all: $(OUTPUT)

# Linking
$(OUTPUT): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) -lgcc

# Compiling C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assembling assembly files
%.o: %.s
	nasm -felf $< 

# Clean up
clean:
	rm -f $(OBJS) $(OUTPUT)

# Phony targets
.PHONY: all clean
