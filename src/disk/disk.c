#include "io/io.h"

int disk_read_sector(int lba, int total, void* buffer) {
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)lba & 0xFF);
    outb(0x1F4, (unsigned char)(lba >> 8) & 0xFF);
    outb(0x1F5, (unsigned char)(lba >> 16) & 0xFF);
    outb(0x1F7, 0x20);

    for (int b = 0; b < total ; b++) {
        unsigned char status = insb(0x1F7);
        while( ! (status & 0x08)) {
            status = insb(0x1F7);
        }
        // copy from hard disk to memory
        for (int i = 0; i < 256 ; i++) {
            *(unsigned short*)buffer = insw(0x1F0);
            buffer += 2;
        }
    }

    return 0;
}
