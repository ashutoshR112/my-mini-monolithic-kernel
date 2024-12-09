
# Minimal Monolithic Kernel

This project is a minimal *monolithic kernel* developed from scratch, based on *[JamesM's](https://archive.is/dWJGu)* and *[Bran's Kernel Development](http://www.osdever.net/bkerndev/Docs/title.htm)* tutorials. The goal of this project is to provide a hands-on learning experience in building a kernel, implementing core functionalities such as memory management, multitasking, and hardware interaction. The kernel is designed to be booted using [GRUB](https://www.gnu.org/software/grub/) and tested in a virtual machine (VM) environment.

![tinyOS in GRUB menu.](https://raw.githubusercontent.com/dilshan/mini-monolithic-kernel/refs/heads/master/docs/grub-tinyos.png)
### Key Features

-  **Cross-compilation Toolchain**: Built using *[GCC](https://gcc.gnu.org/)* and *[Binutils](https://www.gnu.org/software/binutils/)* targeting the *i686-elf* architecture.

-  **GRUB Bootloader**: Uses *GRUB* to create bootable ISO images, replacing traditional floppy booting methods.

-  **Basic Kernel Output**: Initial kernel prints "*Hello, World!*" to the screen in VGA text mode.

-  **Memory Management**: Implements paging and a basic heap for dynamic memory allocation.

-  **Multitasking**: Implements basic multitasking with process scheduling and context switching.

### Kernel Development Process

The development of the kernel is divided into several key sections, each focusing on a different aspect of operating system functionality. Below is a brief overview of the sections covered in the tutorials:

#### 1. [Genesis](https://archive.is/xxLDb)
The kernel starts by setting up a minimal boot environment. It includes bootloader configuration to transition the system from real mode to protected mode and displays the "Hello, World!" message as the first step.

#### 2. [The Screen](https://archive.is/Wg1bR)
The screen output is handled through the VGA text mode, where characters are written directly to video memory. This provides a way to display messages on the screen and serves as a vital debugging tool during kernel development.

```c
#include  "system.h"

int  main(void  *mboot_ptr) {
    printk("Hello World!");

    for (;;);
    return  0xDEADBABA;
}
```
![Hello World example on VM.](https://raw.githubusercontent.com/dilshan/mini-monolithic-kernel/refs/heads/master/docs/tinyos-vgachar-example.png)
#### 3. [The GDT and IDT](https://archive.is/L3pyA)
The *Global Descriptor Table (GDT)* and *Interrupt Descriptor Table (IDT)* are set up to manage memory segments and interrupts. The GDT ensures safe memory access, while the IDT handles hardware and software interrupts, such as timer interrupts and exceptions.

```c
#include  "system.h"

int  main(void  *mboot_ptr) {
    asm  volatile ("int $0x3");
    
    for (;;);
    return  0xDEADBABA;
}
```

#### 4. [IRQs and the PIT](https://archive.is/8W6ew)
Interrupts are enabled using the *Programmable Interrupt Controller (PIC)*, and the *Programmable Interval Timer (PIT)* is configured to generate regular time-based interrupts. These interrupts are critical for managing multitasking and responding to hardware events.

```c
#include  "system.h"
#include  "descriptor_tables.h"
#include  "timer.h"

int  main(void  *mboot_ptr) {
    init_descriptor_tables();
    init_timer(50);
    
    asm  volatile ("sti");
    
    for (;;);
    return  0xDEADBABA;
}
```
![Handling timer interrupts.](https://raw.githubusercontent.com/dilshan/mini-monolithic-kernel/refs/heads/master/docs/tinyos-interrupt-example.png)
#### 5. [Paging](https://archive.is/8MXkb)
Paging is introduced to implement virtual memory, where virtual addresses are mapped to physical memory addresses using page tables. This allows the system to manage memory more effectively, providing isolation between processes and enabling the use of larger address spaces.

```c
#include  "system.h"
#include  "descriptor_tables.h"
#include  "timer.h"
#include  "paging.h"

int  main(void  *mboot_ptr) {
    init_descriptor_tables();
    init_paging();
    
    asm  volatile ("sti");
    uint32_t  *ptr  = (uint32_t*)0xA0000000;
    uint32_t  do_page_fault  =  *ptr;
    printk("value = %d", do_page_fault);
    
    for (;;);
    return  0xDEADBABA;
}
```

#### 6. [The Heap](https://archive.is/zLFOA)
A simple memory allocator is implemented to manage dynamic memory allocation. This allows the kernel to allocate and free memory on the heap, enabling the creation of complex data structures and efficient memory management.

```c
#include  "system.h"
#include  "descriptor_tables.h"
#include  "timer.h"
#include  "paging.h"
#include  "kmalloc.h"

int  main(void  *mboot_ptr) {
    init_descriptor_tables();
    init_paging();
    
    asm  volatile ("sti");
    
    uint32_t  *a  =  kmalloc(8);
    uint32_t  *b  =  kmalloc(8);
    uint32_t  *c  =  kmalloc(8);
    
    *a  =  10;
    *b  =  20;
    *c  =  *a  +  *b;
    
     printk("value = %d", *c);
     kfree(c);
     kfree(b);
     kfree(a);
     
     for (;;);
     return  0xDEADBABA;
}
```

#### 7. [Multitasking](https://archive.is/MhBWU)
Multitasking is implemented by setting up process scheduling and context switching. The kernel can now run multiple processes simultaneously using a basic scheduler.

![Handling thread with main service loop.](https://raw.githubusercontent.com/dilshan/mini-monolithic-kernel/refs/heads/master/docs/tinyos-thread-example.png)
### Building the Kernel

To build the kernel, you need to set up a *cross-compilation toolchain* using *GCC* and *Binutils* targeting the *i686-elf* architecture. The process includes:

1.  **Building Binutils**: Configure and install *Binutils* with support for the *i686-elf* architecture.

2.  **Building GCC**: Compile *GCC* with *i686-elf* support and without standard C libraries.

3.  **Assembling Kernel Code**: Use *[NASM](https://www.nasm.us/)* to assemble the kernel's assembly code.

4.  **Creating Bootable ISO**: Use *GRUB* and the `grub-mkrescue` command to create a bootable ISO image with the kernel.

### Testing the Kernel
Once the kernel is built, it is tested in a *[VirtualBox](https://www.virtualbox.org/)* virtual machine. The virtual machine is configured with:

- Linux (32-bit) as the OS type
- 8MB RAM
- CPU execution cap: approximately 2%
- An optical drive mounted with the bootable ISO image

![Virtual machine configuration.](https://raw.githubusercontent.com/dilshan/mini-monolithic-kernel/refs/heads/master/docs/vm-general-settings.png)

This environment allows you to test the kernel without affecting your host system.

### Running the Kernel

1. Clone the repository:
```bash
git  clone  git@github.com:dilshan/mini-monolithic-kernel.git
cd  mini-monolithic-kernel
```

2. Build the kernel and create the bootable ISO (`tinyos.iso`):
```bash
make
./update_image.sh
```
3. Start VirtualBox and create a new virtual machine. Mount the generated *tinyos.iso* file to the VM's CD/DVD drive and start the VM.

4. The kernel should boot, and you should see the output on the screen (e.g., "*Hello, World!*").
