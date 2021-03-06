/*
    interrupts.h - Header file for x86 interrupts 
*/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "common/common.h"
#include "common/types.h"
#include "kernel/port.h"
#include "kernel/gdt.h"
#include "kernel/multithreading.h"

namespace morphios {
namespace kernel {
    
// IDT consts
static const morphios::common::size_t NUM_INTERRUPTS = 256; // number of entries in IDT (max 256 in x86)
static const morphios::common::uint8_t HW_INTERRUPT_OFFSET = 0x20; // good practice to leave the first 32 IRQs for system interrupts
static const morphios::common::uint8_t IDT_INTERRUPT_GATE = 0xE; // Descriptor type for interrupt gates
static const morphios::common::uint8_t IDT_TRAP_GATE = 0xF; // Descriptor type for trap gates
static const morphios::common::uint8_t IDT_DESC_PRESENT = 0x80;  // Flag to confirm IDT entry is present

// PIC consts
static const morphios::common::uint8_t PIC_MASTER_COMMAND_PORT = 0x20;
static const morphios::common::uint8_t PIC_SLAVE_COMMAND_PORT = 0xA0;
static const morphios::common::uint8_t PIC_MASTER_DATA_PORT = 0x21;
static const morphios::common::uint8_t PIC_SLAVE_DATA_PORT = 0xA1;
static const morphios::common::uint8_t EOI = 0x20; // End Of Interrupt command

class interruptsHandler;

// Non-static class that encapsulates the ISR for each IRQ
class interruptHandle {
protected:
    morphios::common::uint8_t IRQ;
    interruptsHandler *InterruptsHandler;
    interruptHandle(interruptsHandler *InterruptsHandler, morphios::common::uint8_t IRQ);
    ~interruptHandle();
 
 public:
    // Virtual function so it can be overloaded by drivers
    virtual morphios::common::uint32_t ISR(morphios::common::uint32_t esp);
};

// Static class to setup the IDT and handle IRQs
class interruptsHandler {

    friend class interruptHandle;

protected:
    static interruptsHandler* ActiveInterruptsHandler; // Not great, but required to get out of static space
    interruptHandle* interruptHandles[NUM_INTERRUPTS]; // Pointers to interruptHandle objects
    TaskManager *taskManager;

    // IDT gate (entry) definition
    struct Gate {
        morphios::common::uint16_t offsetLo;
        morphios::common::uint16_t selector;
        morphios::common::uint8_t zero;
        morphios::common::uint8_t typeAttr;
        morphios::common::uint16_t offsetHi;
    } __attribute__((packed));

    // IDT
    static Gate interruptDescriptorTable[NUM_INTERRUPTS];

    // IDT pointer
    struct interruptDescriptorTablePointer {
        morphios::common::uint16_t size;
        morphios::common::uint32_t base;
    } __attribute__((packed));

    // Sets the IDT entry for the specified IRQ
    static void SetInterruptDescriptorTableEntry(
        morphios::common::uint8_t IRQ,
        morphios::common::uint16_t codeSegmentSelectorOffset, 
        void (*handler)(),
        morphios::common::uint8_t DescriptorPrivilegeLevel, 
        morphios::common::uint8_t DescriptorType);

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
    
    static void InterruptIgnore(); // Handler for non-handled IRQs

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

    // Top level IRQ handler - called for every interrupt which then calls the below non-static method
    static morphios::common::uint32_t HandleInterrupt(morphios::common::uint8_t IRQ, morphios::common::uint32_t esp);
    
    // Non static version of the above method
    morphios::common::uint32_t DoHandleInterrupt(morphios::common::uint8_t IRQ, morphios::common::uint32_t esp);

    // PIC ports
    Port8Bit PICMasterCommandPort;
    Port8Bit PICSlaveCommandPort;
    Port8Bit PICMasterDataPort;
    Port8Bit PICSlaveDataPort;

public:
    interruptsHandler(GlobalDescriptorTable* globalDescriptorTable, TaskManager *taskManager);
    ~interruptsHandler();
    void Activate();
    void Deactivate();
};

} // namespace kernel    
} // namespace morphios

#endif // INTERRUPTS_H
