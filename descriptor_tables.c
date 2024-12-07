#include "system.h"
#include "descriptor_tables.h"

// Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT) definitions.
gdt_entry_t gdt_entries[5];      // GDT entries
gdt_ptr_t gdt_ptr;              // GDT pointer structure

idt_entry_t idt_entries[256];    // IDT entries
idt_ptr_t idt_ptr;              // IDT pointer structure
interrupt_handler_t interrupt_handlers[256]; // Array of interrupt handlers

/**
 * @brief Set a GDT entry with specified parameters.
 * 
 * @param num    Entry number in the GDT
 * @param base   Base address for the segment
 * @param limit  Segment limit
 * @param access Access flags
 * @param gran   Granularity flags
 */
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

/**
 * @brief Initialize the Global Descriptor Table (GDT).
 */
void init_gdt() {
    // Set GDT pointer size and base
    gdt_ptr.limit = sizeof(gdt_entry_t) * 5 - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Define GDT entries
    gdt_set_gate(0, 0, 0, 0, 0);                  // Null segment
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);      // Code segment
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);      // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);   // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);   // User mode data segment

    // Load the GDT into the CPU
    gdt_flush((uint32_t)&gdt_ptr);
}

/**
 * @brief Set an entry in the Interrupt Descriptor Table (IDT).
 * 
 * @param num   Entry number in the IDT
 * @param base  Base address of the ISR
 * @param sel   Kernel segment selector
 * @param flags Flags for the IDT entry
 */
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags;
}

/**
 * @brief Initialize the Interrupt Descriptor Table (IDT).
 */
void init_idt() {
    // Zero all interrupt handlers initially.
    memset((uint8_t*)&interrupt_handlers, 0, sizeof(interrupt_handler_t) * 256);

    // Set the IDT pointer size and base
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // Zero the IDT to start with.
    memset((uint8_t*)&idt_entries, 0, sizeof(idt_entry_t) * 255);

    // Remap the IRQ table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Set each gate in the IDT.
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    // IRQ handlers
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    // Tell the CPU about our new IDT.
    idt_flush((uint32_t)&idt_ptr);
}

/**
 * @brief Initialize the descriptor tables (GDT and IDT).
 */
void init_descriptor_tables() {
    init_gdt();  // Initialize the GDT
    init_idt();  // Initialize the IDT
}

/**
 * @brief Common handler for interrupts.
 * 
 * @param regs Pointer to the register state when the interrupt occurred.
 */
void idt_handler(registers_t* regs) {
    if (interrupt_handlers[regs->int_no]) {
        interrupt_handlers[regs->int_no](regs);  // Call the registered handler
    } else {
        printk("Unhandled interrupt: %d\n", regs->int_no);
        panic("Unhandled interrupt");
    }
}

/**
 * @brief Register an interrupt handler for a specific interrupt number.
 * 
 * @param n Interrupt number to associate with the handler.
 * @param h Function pointer to the interrupt handler.
 */
void register_interrupt_handler(uint8_t n, interrupt_handler_t h) {
    interrupt_handlers[n] = h;  // Register the handler for the interrupt
}

/**
 * @brief Common handler for hardware interrupts (IRQs).
 * 
 * This function is called from the assembly interrupt handler stub. It processes
 * the interrupt, sends End of Interrupt (EOI) signals to the Programmable Interrupt Controller (PIC),
 * and invokes the appropriate registered interrupt handler.
 * 
 * @param regs Pointer to the register state when the interrupt occurred.
 */
void irq_handler(registers_t *regs) {
    // Send an End of Interrupt (EOI) signal to the PICs.
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20);  // Reset slave PIC
    }
    outb(0x20, 0x20);  // Reset master PIC

    // Check if a handler is registered for this interrupt and call it if it exists.
    if (interrupt_handlers[regs->int_no] != 0) {
        interrupt_handlers[regs->int_no](regs);  // Call the handler
    }
}
