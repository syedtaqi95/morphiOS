/*
    interrupts.cpp - x86 interrupt handler in C++
*/

#include "interrupts.h"

// Used for debug printing
void printf(const char* str);

char ascii_values[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// IDT
interruptsHandler::Gate interruptsHandler::interruptDescriptorTable[256];


void interruptsHandler::SetInterruptDescriptorTableEntry(uint8_t interrupt,
    uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType) {
    // address of pointer to code segment (relative to global descriptor table)
    // and address of the handler (relative to segment)
    interruptDescriptorTable[interrupt].offsetLo = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].offsetHi = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].selector = CodeSegment;
    interruptDescriptorTable[interrupt].typeAttr = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 0x3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].zero = 0;
}


interruptsHandler::interruptsHandler(GlobalDescriptorTable* globalDescriptorTable) {
    
    uint32_t CodeSegment = globalDescriptorTable->CodeSegmentSelector();

    // Initialise all interrupts to default (do nothing)
    for (uint8_t i = 255; i > 0; i--) {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    }

    // Set first 32 IRQs to system exceptions
    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandlerException0x00, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandlerException0x01, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandlerException0x02, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandlerException0x03, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandlerException0x04, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandlerException0x05, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandlerException0x06, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandlerException0x07, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandlerException0x08, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandlerException0x09, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandlerException0x0A, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandlerException0x0B, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandlerException0x0C, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandlerException0x0D, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandlerException0x0E, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandlerException0x0F, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandlerException0x10, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandlerException0x11, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandlerException0x12, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x14, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x15, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x16, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x17, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x18, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x19, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1A, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1B, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1C, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1D, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1E, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);
    SetInterruptDescriptorTableEntry(0x1F, CodeSegment, &HandlerException0x13, 0, IDT_TRAP_GATE);

    // User defined PIC interrupts
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x00, CodeSegment, &HandlerIRQ0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x01, CodeSegment, &HandlerIRQ0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x02, CodeSegment, &HandlerIRQ0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x03, CodeSegment, &HandlerIRQ0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x04, CodeSegment, &HandlerIRQ0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x05, CodeSegment, &HandlerIRQ0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x06, CodeSegment, &HandlerIRQ0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x07, CodeSegment, &HandlerIRQ0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x08, CodeSegment, &HandlerIRQ0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x09, CodeSegment, &HandlerIRQ0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0A, CodeSegment, &HandlerIRQ0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0B, CodeSegment, &HandlerIRQ0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0C, CodeSegment, &HandlerIRQ0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0D, CodeSegment, &HandlerIRQ0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0E, CodeSegment, &HandlerIRQ0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(HW_INTERRUPT_OFFSET + 0x0F, CodeSegment, &HandlerIRQ0x0F, 0, IDT_INTERRUPT_GATE);
    
    // Remap the PIC
    Port8Bit PICMasterCommandPort(0x20);
    Port8Bit PICSlaveCommandPort(0xA0);
    Port8Bit PICMasterDataPort(0x21);
    Port8Bit PICSlaveDataPort(0xA1);

    PICMasterCommandPort.write(0x11);
    PICSlaveCommandPort.write(0x11);

    PICMasterDataPort.write(HW_INTERRUPT_OFFSET);
    PICSlaveDataPort.write(HW_INTERRUPT_OFFSET+8);

    PICMasterDataPort.write(0x04);
    PICSlaveDataPort.write(0x02);

    PICMasterDataPort.write(0x01);
    PICSlaveDataPort.write(0x01);

    PICMasterDataPort.write(0x00);
    PICSlaveDataPort.write(0x00);

    // Load IDT register
    interruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(Gate) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
}


interruptsHandler::~interruptsHandler(){}


void interruptsHandler::Activate() {
    asm("sti");
}

uint32_t interruptsHandler::HandleInterrupt(uint8_t interrupt, uint32_t esp) {

    // Debug print
    // char* message = "INTERRUPT 0x00 ";
    // char* hex = "0123456789ABCDEF";
    // message[12] = hex[(interrupt >> 4) & 0xF];
    // message[13] = hex[interrupt & 0xF];
    // printf(message);

    if (interrupt == 0x21) {
        Port8Bit keyboardDataPort(0x60);
        uint8_t scanCode = keyboardDataPort.read();
        if (scanCode < 0x80) {
            // char *c = "KEYBOARD 0x00 ";
            // char *hex = "01234567889ABCDEF";
            // c[11] = hex[(scanCode >> 4) & 0x0F];
            // c[12] = hex[scanCode & 0x0F];
            char *c = " ";
            c[0] = ascii_values[scanCode];

            printf(c);
        }
        

    }

    // EOI (End Of Interrupt)
    Port8Bit MasterCommandPort(0x20);
    Port8Bit SlaveCommandPort(0xA0);
    if (interrupt >= (HW_INTERRUPT_OFFSET + 8))
        SlaveCommandPort.write(EOI);
    MasterCommandPort.write(EOI);

    return esp;
}

void interruptsHandler::InterruptIgnore(){
    asm volatile("iret");
}

// void interruptsHandler::ISR0x00(void) {

// }

