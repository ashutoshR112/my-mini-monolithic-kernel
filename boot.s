; Declare constants for the multiboot header
%define ALIGN       1 << 0           ; align loaded modules on page boundaries
%define MEMINFO     1 << 1           ; provide memory map
%define FLAGS       (ALIGN | MEMINFO); this is the Multiboot 'flag' field
%define MAGIC       0x1BADB002       ; 'magic number' lets bootloader find the header
%define CHECKSUM    -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

; Declare a multiboot header
section .multiboot
align 4
dd MAGIC          ; Magic number
dd FLAGS          ; Flags
dd CHECKSUM       ; Checksum

; Allocate space for the stack
section .bss
align 32
stack_bottom:
resb 32768        ; Reserve 32 KiB for the stack
stack_top:

; Kernel entry point
section .text
global _start
extern main                    ; This is the entry point of our C kernel code

_start:
    ; Set up the stack
    mov esp, stack_top

    ; Push the initial values to the stack
    push ebx          ; Save the bootloader information
    xor ebp, ebp      ; Clear the base pointer

    ; Call the main kernel function
    call main

    ; Infinite loop to halt the system
    cli               ; Disable interrupts
.loop:
    hlt               ; Halt the CPU
    jmp .loop         ; Loop forever

_start_end:
