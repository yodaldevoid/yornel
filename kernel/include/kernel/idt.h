#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

#define IDT_SIZE 256

#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xA0
#define PIC2_DAT 0xA1

// IDT - Interupt Descriptor Table - matches interupt number to interupt handler address
struct IDT_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
} __attribute__((packed)); // makes sure that gcc doesn't add padding for any reason

void idt_init(void);

void interrupt_enable(void);

#endif