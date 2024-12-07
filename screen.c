#include "screen.h"
#include "system.h"

#define MON_DEFAULT_ATTR_BYTE  ((0 /* black */ << 4) | (15 /* white */ & 0x0F))
#define MON_BLANK_CHAR         (0x20 /* space */ | (MON_DEFAULT_ATTR_BYTE << 8))

#define MON_WIDTH  80
#define MON_HEIGHT 25

static void move_csr(void);
static void scroll(void); 

static uint16_t *textmemptr = (uint16_t *)0xB8000;
// Stores the cursor position.
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_csr(void)
{
    /* cursor location */
    uint16_t cl = cursor_y * MON_WIDTH + cursor_x;
    outb(0x3D4, 14);      /* Tell the VGA board we are setting the high cursor byte. */
    outb(0x3D5, cl >> 8); /* Send the high cursor byte. */
    outb(0x3D4, 15);      /* Tell the VGA board we are setting the low cursor byte. */
    outb(0x3D5, cl);      /* Send the low cursor byte. */
}

static void scroll(void)
{
    int i, w = MON_WIDTH, h = MON_HEIGHT;

    if (cursor_y >= h) { 
        for (i = 0; i < w * (h - 1); i++) {
            textmemptr[i] = textmemptr[i + w];
        }

        for (i = w * (h - 1); i < w * h; i++) {
            textmemptr[i] = MON_BLANK_CHAR;
        }
  
        cursor_y = (h - 1);
    }
}

void putch(char c)
{
    uint8_t bc = 0, fc = 2;


    uint8_t attr_byte = (bc << 4) | (fc & 0x0F);

    uint16_t attr = attr_byte << 8;
    uint16_t *location;

    if (c == 0x08 && cursor_x > 0) {

        cursor_x--;
    } else if (c == 0x09) {

        cursor_x = (cursor_x + 8) & ~(8 - 1);
    } else if (c == '\r') {

        cursor_x = 0;
    } else if (c == '\n') {

        cursor_x = 0;
        cursor_y++;
    } else if (c >= ' ') {

        location = textmemptr + (cursor_x++ + MON_WIDTH * cursor_y);
        *location = c | attr;
    }

    if (cursor_x >= MON_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    scroll();
    move_csr();
}

void cls(void)
{
    int i;

    for (i = 0; i < MON_WIDTH * MON_HEIGHT; i++) {
        textmemptr[i] = MON_BLANK_CHAR;
    }
    cursor_x = cursor_y = 0;
    move_csr();
}

void screen_write(char *c)
{
    int i = 0;
    while (c[i]) {
        putch(c[i++]);
    }
}
