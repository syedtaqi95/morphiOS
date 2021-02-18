/*
    interrupts.h - Header file for x86 interrupts 
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

#define NUM_INTERRUPTS 256 // number of entries in IDT (max 256 in x86)
#define HW_INTERRUPT_OFFSET 0x20 // good practice to leave the first 32 IRQs for system interrupts
#define IDT_INTERRUPT_GATE 0xE // Descriptor type for interrupt gates
#define IDT_TRAP_GATE 0xF // Descriptor type for trap gates
#define IDT_DESC_PRESENT 0x80 



class interruptsHandler {

    protected:
        
        // IDT gate (entry) definition
        struct Gate {
            uint16_t offsetLo;
            uint16_t selector;
            uint8_t zero;
            uint8_t typeAttr;
            uint16_t offsetHi;
        } __attribute__((packed));

        // IDT
        static Gate interruptDescriptorTable[NUM_INTERRUPTS];

        // IDT pointer
        struct interruptDescriptorTablePointer {
            uint16_t size;
            uint32_t base;
        } __attribute__((packed));

        // Sets the entry for the particular interrupt
        static void SetInterruptDescriptorTableEntry(
            uint8_t interrupt,
            uint16_t codeSegmentSelectorOffset, 
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel, 
            uint8_t DescriptorType);

        // Interrupt handlers
        // 0x00...0x0F are system reserved interrupts
        static void HandlerIRQ0x00();
        static void HandlerIRQ0x01();
        static void HandlerIRQ0x02();
        static void HandlerIRQ0x03();
        static void HandlerIRQ0x04();
        static void HandlerIRQ0x05();
        static void HandlerIRQ0x06();
        static void HandlerIRQ0x07();
        static void HandlerIRQ0x08();
        static void HandlerIRQ0x09();
        static void HandlerIRQ0x0A();
        static void HandlerIRQ0x0B();
        static void HandlerIRQ0x0C();
        static void HandlerIRQ0x0D();
        static void HandlerIRQ0x0E();
        static void HandlerIRQ0x0F();
        static void HandlerIRQ0x31(); // Keyboard device interrupt

        static void InterruptIgnore(); // Default handler

        // Exception handlers
        static void HandlerException0x00();
        static void HandlerException0x01();
        static void HandlerException0x02();
        static void HandlerException0x03();
        static void HandlerException0x04();
        static void HandlerException0x05();
        static void HandlerException0x06();
        static void HandlerException0x07();
        static void HandlerException0x08();
        static void HandlerException0x09();
        static void HandlerException0x0A();
        static void HandlerException0x0B();
        static void HandlerException0x0C();
        static void HandlerException0x0D();
        static void HandlerException0x0E();
        static void HandlerException0x0F();
        static void HandlerException0x10();
        static void HandlerException0x11();
        static void HandlerException0x12();
        static void HandlerException0x13();

        // Generic "boilerplate" IRQ handler - called for every interrupt
        static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp); 
        
        // ISRs
        static void ISR0x00();

        // Master and slave PIC ports for COMMAND and DATA
        Port8Bit programmableInterruptControllerMasterCommandPort;
        Port8Bit programmableInterruptControllerMasterDataPort;
        Port8Bit programmableInterruptControllerSlaveCommandPort;
        Port8Bit programmableInterruptControllerSlaveDataPort;
    
    public:

        interruptsHandler(GlobalDescriptorTable* globalDescriptorTable);
        ~interruptsHandler();
        void Activate();
};

#endif // INTERRUPTS_H