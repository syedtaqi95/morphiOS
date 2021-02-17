/*
    interrupts.cpp
*/

#include "interrupts.h"


void printf(const char* str);


interruptsHandler::Gate interruptsHandler::interruptDescriptorTable[256];


void interruptsHandler::SetInterruptDescriptorTableEntry(uint8_t interrupt,
    uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType)
{
    // address of pointer to code segment (relative to global descriptor table)
    // and address of the handler (relative to segment)
    interruptDescriptorTable[interrupt].offsetLo = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].offsetHi = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].selector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interrupt].typeAttr = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].zero = 0;
}


interruptsHandler::interruptsHandler(GlobalDescriptorTable* globalDescriptorTable)
    : programmableInterruptControllerMasterCommandPort(0x20),
      programmableInterruptControllerMasterDataPort(0x21),
      programmableInterruptControllerSlaveCommandPort(0xA0),
      programmableInterruptControllerSlaveDataPort(0xA1)
{
    uint32_t CodeSegment = globalDescriptorTable->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint8_t i = 255; i > 0; --i)
    {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDescriptorTableEntry(0, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandlerException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandlerException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandlerException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandlerException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandlerException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandlerException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandlerException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandlerException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandlerException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandlerException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandlerException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandlerException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandlerException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandlerException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandlerException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandlerException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandlerException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandlerException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandlerException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandlerException0x13, 0, IDT_INTERRUPT_GATE);

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

    programmableInterruptControllerMasterCommandPort.write(0x11);
    programmableInterruptControllerSlaveCommandPort.write(0x11);

    // remap
    programmableInterruptControllerMasterDataPort.write(HW_INTERRUPT_OFFSET);
    programmableInterruptControllerSlaveDataPort.write(HW_INTERRUPT_OFFSET+8);

    programmableInterruptControllerMasterDataPort.write(0x04);
    programmableInterruptControllerSlaveDataPort.write(0x02);

    programmableInterruptControllerMasterDataPort.write(0x01);
    programmableInterruptControllerSlaveDataPort.write(0x01);

    programmableInterruptControllerMasterDataPort.write(0x00);
    programmableInterruptControllerSlaveDataPort.write(0x00);

    interruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(Gate) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
}

interruptsHandler::~interruptsHandler(){}

void interruptsHandler::Activate()
{
    asm("sti");
}

uint32_t interruptsHandler::HandleInterrupt(uint8_t interrupt, uint32_t esp)
{
    char* foo = "INTERRUPT 0x00";
    char* hex = "0123456789ABCDEF";

    foo[12] = hex[(interrupt >> 4) & 0xF];
    foo[13] = hex[interrupt & 0xF];
    printf(foo);

    return esp;
}