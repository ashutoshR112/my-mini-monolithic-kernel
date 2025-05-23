#include "keyboard.h"
#include "descriptor_tables.h"
#include "system.h"
#include "screen.h"

void keyboard_handler(struct regs *r);

volatile char key_buffer[256];
volatile int key_index = 0;

void init_keyboard() {
    register_interrupt_handler(33, keyboard_handler); // IRQ1 = 33
    outb(0x21, inb(0x21) & ~0x02); // Unmask IRQ1
}

char scancode_to_ascii(unsigned char scancode) {
    static char kbdus[128] = {
        0,  27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
        'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
        'z','x','c','v','b','n','m',',','.','/', 0, '*',
        0,  ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    if (scancode > 127) return 0;
    return kbdus[scancode];
}

void keyboard_handler(struct regs *r) {
    unsigned char scancode = inb(0x60);
    char key = scancode_to_ascii(scancode);
    if (key && key_index < 255) {
        key_buffer[key_index++] = key;
        key_buffer[key_index] = '\0';
    }

    // Debug feedback on screen
    screen_write_at("Key IRQ1", 0, 0);
}

char get_last_key() {
    if (key_index == 0) return 0;
    char key = key_buffer[0];
    for (int i = 0; i < key_index; i++)
        key_buffer[i] = key_buffer[i + 1];
    key_index--;
    return key;
}

int is_key_ready() {
    return key_index > 0;
}
