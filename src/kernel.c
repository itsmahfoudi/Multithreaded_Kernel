#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "disk/streamer.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;


uint16_t terminal_make_char(char c, char color) 
{
  return (color << 8) | c;
}

void terminal_put_char(int x, int y, char character, char color) 
{
  video_mem[(y*VGA_WIDTH) + x] = terminal_make_char(character, color);
}

void terminal_writechar(char c, char color) 
{
  if (c == '\n') {
    terminal_row += 1;
    terminal_col = 0;
    return;
  }
  terminal_put_char(terminal_col, terminal_row, c, color);
  terminal_col += 1;
  if (terminal_col >= VGA_WIDTH) 
  {
    terminal_col = 0;
    terminal_row += 1;
  }
}

void print(const char * str) 
{
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++) {
    terminal_writechar(str[i], 15);
  }
}

void terminal_initialize(void) 
{
  video_mem = (uint16_t*)0xB8000;
  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      video_mem[(y*VGA_WIDTH) + x] = terminal_make_char(' ', 0);
    }
  }
}
extern void problem(void);

static paging_4gb_chunk_t* kernel_chunk = 0;

void kernel_main(void) {
    terminal_initialize();
    print("AYOUB EL MAHFOUDI\n");

    // Initialize the heap
    print("Initialize the kernel heap\n");
    kheap_init();

    // Initialize the disk
    print("Initialize the disk\n");
    disk_search_and_init();

    //Initializing the interrupt descriptor table
    print("Initialize the IDT\n");
    idt_init();

    //creating page tables
    print("Enable paging\n");
    kernel_chunk = paging_new_4gb(PAGING_IS_PRESENT | PAGING_IS_WRITABLE | PAGING_ACCESS_FROM_ALL);
    // switch to kernel paging chunk
    paging_switch(kernel_chunk->directory_entry);

    //enable paging
    enable_paging();

    //Enable the system interrupts
    enable_interrupts();

    // test the disk streamer
    disk_stream *stream = disk_streamer_create(0);
    if (stream == NULL) {
        print("Failed to create disk stream\n");
        while (1);
        //problem();
    }
    disk_streamer_seek(stream, 0x40b);
    unsigned char buff;
    disk_streamer_read(stream, &buff, 4);
    while (1);
}
