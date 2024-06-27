#ifndef KERNEL_H
#define KERNEL_H

#define VGA_HEIGHT 20
#define VGA_WIDTH  80

void terminal_initialize(void);
void kernel_main(void);
void print(const char * str); 

#endif // !KERNEL_H
