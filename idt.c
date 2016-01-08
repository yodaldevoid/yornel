#include "idt.h"

#include <stddef.h>
#include "gdt.h"
#include "io.h"

static void load_idt(void* base, size_t size) {
	struct {
		uint16_t limit;
		uintptr_t base;
	} __attribute__((packed)) IDTR = {(uint16_t) size - 1, (uintptr_t) base};
	
	asm("lidt %0": : "m" (IDTR));
	asm("sti");
}

extern void keyboard_handler(void);

void idt_init(void) {
	uintptr_t keyboard_address;
	
	// populate IDT entry for keyboard interrupt
	keyboard_address = (uintptr_t) keyboard_handler;
	IDT[0x21].offset_low = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CS;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; // INTERRUPT_GATE
	IDT[0x21].offset_high = (keyboard_address & 0xffff0000) >> 16;
	
	// ICW1 - begin init - sets to wait for three more bytes on data ports
	outb(PIC1_CMD, 0x11);
	outb(PIC2_CMD, 0x11);
	
	// ICW2 - remap offset of IDT because first 32 interrupts are for CPU
	outb(PIC1_DAT, 0x20);
	outb(PIC2_DAT, 0x28);
	
	// ICW3 - set everything with no slaves
	outb(PIC1_DAT, 0x00);
	outb(PIC2_DAT, 0x00);
	
	// ICW4 - enviroment info - 80x86 mode
	outb(PIC1_DAT, 0x01);
	outb(PIC2_DAT, 0x01);
	
	// mask interupts
	outb(PIC1_DAT, 0xff);
	outb(PIC1_DAT, 0xff);
	
	load_idt(IDT, sizeof(IDT));
}

